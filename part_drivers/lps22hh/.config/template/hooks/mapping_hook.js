/**
  ******************************************************************************
  * @file    mapping_hook.js
  * @author  GPAM Application Team
  * @brief   This file implements functions providing the mapping logic to generate code.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/**
 * Nominal hook as per embedded software architecture
 *
 * We expect the codegen service to call this hook per component.
 *
 * @param {string}   component_id         identifier of the component being processed by the codegen service
 * @param {array}    component_templates  array of templates for component_id
 * @param {function} _get_hw_instances     function returning the hardware instances (from the configuration data model) associated to the component id
 * @param {function} get_global_getters_registry     function returning the get_global_getters_registry
 * @param {object}   _cfg_data             configuration data for the current software project
 * @param {string}   _secure_ctxt          indicates if the context is 'None' (no security), 'Secure' (secure context), 'Non-secure' (non-secure context)
 * @param {string}   _genfileslist_folder  where to write the list of files to be generated
 * @param {function} debug_print           function to evacuate logs
 * @return object describing the mapping rules to be applied, empty object if nothing to process
 */
module.exports.main_grouping_hook = function (
  component_id,
  component_templates,
  _get_hw_instances,
  get_global_getters_registry,
  _cfg_data,
  _secure_ctxt,
  _genfileslist_folder,
  debug_print,
  _strategy
) {
  /**
   * This function finds out which files must be generated based on the field info.interface (as implemented in .config/mx_*_parameters.json).
   * LPS22HH instances can use either a built-in IO interface (I2C or SPI), or the "custom" interface.
   * Built-in interfaces use codegen templates; the custom interface does not, but uses the copy-once mechanism instead.
   */

  let instances_configs = [];
  try {
    const globalGetters = get_global_getters_registry();
    const swConfigGettersAPI = globalGetters.swConfigGettersAPI.SWConfigurationAPI;
    const instancesIds = swConfigGettersAPI.getInstances(component_id);
    instances_configs = instancesIds.map(id => swConfigGettersAPI.getSwInstanceConfiguration(id));
  } catch (error) {
    debug_print(`Error in mapping hook for component ${component_id} : ${error.message}`);
  }

  let Is_Builtin_Used = false;
  let Is_Custom_Used = false;

  for (const instance of instances_configs) {
    switch (instance.info.interface) {
      case 'I2C':
      case 'SPI':
        Is_Builtin_Used = true;
        break;
      case 'Custom':
        Is_Custom_Used = true;
        break;
      default:
        break;
    }
  }

  let hook_result = [];
  if (Is_Builtin_Used) {
    for (const template of component_templates) {
      hook_result.push(getHookResult(component_id, template, getTemplateName(template)));
    }
  }
  if (Is_Custom_Used) {
    /* no template is pushed and the list remains empty */
  }

  debug_print("################ LPS22HH HOOK ################");
  debug_print("component_templates", component_templates);
  debug_print("instances_configs", instances_configs);
  debug_print("Is_Builtin_Used", Is_Builtin_Used);
  debug_print("Is_Custom_Used", Is_Custom_Used);
  debug_print("hook_result", hook_result);
  return hook_result;
}


function getTemplateName(templateFileName) {
  return templateFileName.replace('_template', '').replace('.hbs', '');
}
function getHookResult(component_id, templateName, outputFileName) {
  return {
    component: component_id,
    resource_type: 'COMPONENT_ENTRY',
    resource: 'no hardware resource',
    template: templateName,
    output: outputFileName,
  };
}
