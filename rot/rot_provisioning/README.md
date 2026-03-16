# RoT Provisioning Software Pack

![tag](https://img.shields.io/badge/tag-v2.0.0-brightgreen.svg)
[![release note](https://img.shields.io/badge/release_note-view_html-gold.svg)](Release_Notes.html)

## Overview

Provisioning framework and shared logic for Root of Trust based examples.

This package provides:

- Core RoT logic for memory flash layouts, image preparation, and device provisioning across multiple STM32 RoT use cases.
- Ready-to-use entry-point scripts that let RoT examples trigger full provisioning and prebuild/postbuild commands.

## Description

This package centralizes the common logic needed to provision RoT examples and to run associated prebuild/postbuild steps. It is designed to be called by example projects, which integrate it into their own provisioning and build flows rather than invoking individual scripts manually.

In particular, it provides:

- **RoT Provisioning primitives**: Shared scripts to compute memory flash layouts and prepare images and related artifacts consumed by RoT examples.
- **Example integration support**: Building blocks used by example-specific provisioning scripts.

Scripts in this package are intended to be used programmatically by examples, not as standalone end-user tools.

## Usage

In normal use, you do not call this package directly. Instead, it is used by RoT example projects as part of their provisioning flow.

1. Open the RoT example project you want to run.
2. Follow that project's `README.md`, which will:
   - Guide you to set up your environment (Python, Python modules, other dependencies).
   - Invoke the provisioning commands that internally rely on RoT Provisioning.

If you are integrating these helpers into a new project, refer to existing RoT examples for reference on how they import and invoke the scripts from this package.

