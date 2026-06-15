---
pagetitle: Release Notes for LPS22HH part driver
lang: en
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for <mark>lps22hh_part_drivers</mark>
</center>

# Purpose

The part driver is the interface for an external device on the board. It does not directly depend on the HAL nor on the board hardware design. This driver provides specific unitary services and could be used by any board integrating this external component.

The principle of the STM32Cube2 part drivers' architecture is intended to:

- Make the parts drivers easily used directly by applicative code
with better glue logic between components and bus.
- Separate the driver core from resources configuration and definition.
- Have direct services call from application level without using driver and class data structures.

:::

::: {.col-sm-12 .col-lg-8}
# Update history

::: {.collapse}
<input type="checkbox" id="collapse-v-1-0-0" aria-hidden="true">
<label for="collapse-v-1-0-0" aria-hidden="true">**1.0.0 / 18-February-2026**</label>
<div>


## Main changes

### First release

Initial release of the LPS22HH part driver, including support for configuration and code generation in STM32CubeMX2.

## Known limitations

No limitation raised for this part driver.

## Dependencies

Configuration and code generation features require STM32CubeMX2 >= 0.1.621.

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on STM32 Microcontrollers </mark> ,
visit: [[www.st.com/stm32](http://www.st.com/stm32)]{style="font-color: blue;"}

This release note uses up to date web standards and, for this reason, should not
be opened with Internet Explorer but preferably with popular browsers such as
Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.
</footer>
