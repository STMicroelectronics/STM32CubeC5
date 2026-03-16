#
# ****************************************************************************
# @attention
#
# Copyright (c) 2026 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
#
# ****************************************************************************

import xml.etree.ElementTree as ET
import os
import logging

from helper.exception.rot_exception import RoTNotFoundError

logger = logging.getLogger()


def _build_absolute_path_from_xml(xml, value_text):
    return os.path.abspath(os.path.join(os.path.dirname(xml), value_text))


def _extract_value_from_xml(xml):
    tree = ET.parse(xml)
    root = tree.getroot()
    value_tag = root.find(".//Output/Value")
    if value_tag is None or value_tag.text is None:
        raise RoTNotFoundError(f"No '<Output><Value>' found in XML file: {xml}.")
    return value_tag.text


def extract_output_file_path(xml):
    """
    Extracts the output file path from an XML file.
    Absolute path is always returned. If the path in the XML is relative,
    it is converted to an absolute path based on the XML file's location.

    Example XML structure:
    <Root>
        <Output>
            <Value>relative/or/absolute/path/to/binary.bin</Value>
        </Output>
    </Root>

    Args:
        xml (str): Path to the XML file.
    Returns:
        str: Absolute path to the output binary file.
    Raises:
        RoTNotFoundError: If the <Value> tag is not found within <Output> or is empty.
    """
    output_value = _extract_value_from_xml(xml)

    if (os.path.isabs(output_value)):
        return output_value
    return _build_absolute_path_from_xml(xml, output_value)


def _update_xml_value(xml_path: str, tag_name: str, field_name_tag: str, field_name: str, field_to_update: str, new_value: str) -> None:
    """
    Updates the value of a field in an XML file based on the field name and its tag (e.g., Name, Title).
    All the empty field <NAME_FIELD><NAME_FIELD/> are updated with <NAME_FIELD />.

    Args:
        xml_path (str): Path to the XML file.
        tag_name (str): The tag to search for (e.g., "Param", "Output").
        field_name (str): The name of the field to update (e.g., "Firmware area Size").
        field_name_tag (str): The tag to search for the field name (e.g., "Name", "Title").
        field_to_update (str): The field of the value to update (e.g., "Value").
        new_value (str): The new value to assign to the field. Can be a path, size, or address.

    Assumptions:
     - The file path is an existing absolute path (no check is performed).

    Raises:
        RoTNotFoundError: If the field with the given name is not found in the XML file.
    """
    # Parse the XML file
    tree = ET.parse(xml_path)
    root = tree.getroot()

    # Find the field by name in the specified tag (e.g., Name or Title)
    field = root.find(f".//{tag_name}[{field_name_tag}='{field_name}']")
    if field is None:
        raise RoTNotFoundError(f"The field {field_name_tag}='{field_name}' was not found in the '{xml_path}' file.")

    # Update the field value
    value_element = field.find(field_to_update)
    if value_element is None:
        raise RoTNotFoundError(
            f"The field {field_name_tag}='{field_name}' does not have a '{field_to_update}' element to update.")

    value_element.text = new_value
    # Serialize the XML tree to a string
    xml_string = ET.tostring(root, encoding='unicode')

    # Replace single quotes with double quotes in the XML declaration
    xml_string = xml_string.replace("encoding='UTF-8'", 'encoding="UTF-8"')

    # Write the modified XML content back to the file
    with open(xml_path, 'w', encoding='UTF-8') as xml_file:
        xml_file.write('<?xml version="1.0" encoding="UTF-8"?>\n')
        xml_file.write(xml_string)

    logger.debug(
        f"Updated the field '{field_to_update}' in '{tag_name}'/{field_name_tag}='{field_name}' to '{new_value}' in '{xml_path}'.")


def update_xml_value_by_name(xml_path: str, tag_name: str, field_name: str, field_to_update: str, new_value: str) -> None:
    """
    Updates the value of a field in an XML file where the field name is specified by the <Name> tag.

    This is a convenience wrapper for update_xml_value, defaulting the field name tag to 'Name'.

    Args:
        xml_path (str): Path to the XML file.
        tag_name (str): The tag to search for (e.g., "Param", "Output").
        field_name (str): The name of the field to update (matched in <Name> tag).
        field_to_update (str): The field of the value to update (e.g., "Value").
        new_value (str): The new value to assign to the field.
    """
    _update_xml_value(xml_path, tag_name, "Name", field_name, field_to_update, new_value)


def update_xml_value_by_title(xml_path: str, tag_name: str, field_name: str, field_to_update: str, new_value: str) -> None:
    """
    Updates the value of a field in an XML file where the field name is specified by the <Title> tag.

    This is a convenience wrapper for update_xml_value, defaulting the field name tag to 'Title'.

    Args:
        xml_path (str): Path to the XML file.
        tag_name (str): The tag to search for (e.g., "Param", "Output").
        field_name (str): The name of the field to update (matched in <Title> tag).
        field_to_update (str): The field of the value to update (e.g., "Value").
        new_value (str): The new value to assign to the field.
    """
    _update_xml_value(xml_path, tag_name, "Title", field_name, field_to_update, new_value)
