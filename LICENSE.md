

<a name="top"></a>

# These are full license terms for

# <mark>STM32CubeC5</mark>

Copyright &copy; 2026 STMicroelectronics

All rights reserved
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com)

## <mark>__OVERVIEW__</mark>
<div>
This software package is provided under the below SLA.

This table lists the software components in this
software package, including the copyright owner and license terms for each
component.

The full text of these licenses are below the table.

__SOFTWARE COMPONENTS TABLE__

| Software Component | Copyright                                                                       | License
|--------------------|----------                                                                       |--------
| stm32c5xx_DFP | [See the copyrights statements](#cp1) | [BSD-3-Clause](#lic2)
| stm32c5xx_HAL_Drivers | [See the copyrights statements](#cp2) | [BSD-3-Clause](#lic2)
| button_Part_Drivers | [See the copyrights statements](#cp3) | [BSD-3-Clause](#lic2)
| lan8742_Part_Drivers | [See the copyrights statements](#cp4) | [BSD-3-Clause](#lic2)
| led_Part_Drivers | [See the copyrights statements](#cp5) | [BSD-3-Clause](#lic2)
| w25n01gvxx_Part_Drivers | [See the copyrights statements](#cp6) | [BSD-3-Clause](#lic2)
| w25q128j_Part_Drivers | [See the copyrights statements](#cp7) | [BSD-3-Clause](#lic2)
| ARM-software/CMSIS-DSP | [See the copyrights statements](#cp8) | [Apache-2.0](#lic3)
| ARM-software/CMSIS_6 | [See the copyrights statements](#cp9) | [Apache-2.0](#lic3)
| Advanced_Trace | [See the copyrights statements](#cp10) | [BSD-3-Clause](#lic2)
| Basic_STDIO | [See the copyrights statements](#cp11) | [BSD-3-Clause](#lic2)
| EEPROM_Emulation | [See the copyrights statements](#cp12) | [BSD-3-Clause](#lic2)
| Sequencer | [See the copyrights statements](#cp13) | [BSD-3-Clause](#lic2)
| Syscalls | [See the copyrights statements](#cp14) | [BSD-3-Clause](#lic2)
| FreeRTOS Real Time Kernel | [See the copyrights statements](#cp15) | [MIT](#lic4)
| MbedTLS | [See the copyrights statements](#cp16) | [Apache-2.0](#lic3)
| STCryptoLib | [See the copyrights statements](#cp17) | [SLA0044 Rev6/October 2025](#lic1)
| STFCF | [See the copyrights statements](#cp18) | [Apache-2.0](#lic3)
| eclipse-threadx/filex | [See the copyrights statements](#cp19) | [MIT](#lic4)
| eclipse-threadx/levelx | [See the copyrights statements](#cp20) | [MIT](#lic4)
| eclipse-threadx/usbx_STM32 | [See the copyrights statements](#cp21) | [MIT](#lic4)
| git.trustedfirmware.org/TF-M/trusted-firmware-m | [See the copyrights statements](#cp22) | [Apache-2.0](#lic3)
| lwIP | [See the copyrights statements](#cp23) | [BSD-3-Clause](#lic2)
| RoT_Provisioning | [See the copyrights statements](#cp24) | [SLA0044 Rev6/October 2025](#lic1)
| STM32Cube software example | Copyright (c) 2026 STMicroelectronics | [SLA0044 Rev6/October 2025](#lic1)

__Notes:__ The full license terms are below. 

If a component is not listed in the table above, then the SLA
shall apply unless other terms are clearly stated in the package.

</div>

<label for="collapse-sla" aria-hidden="true">__SLA – Software License Agreement__</label>
<div>

SLA0048 Rev5/October 2025

## Software license agreement

### __SOFTWARE PACKAGE LICENSE AGREEMENT (“AGREEMENT”)__

BY CLICKING ON THE "I ACCEPT" BUTTON OR BY  UNZIPPING, INSTALLING, COPYING, DOWNLOADING, ACCESSING OR OTHERWISE USING THIS SOFTWARE PACKAGE OR ANY PART THEREOF, INCLUDING ANY RELATED DOCUMENTATION (collectively the “SOFTWARE PACKAGE”) FROM STMICROELECTRONICS INTERNATIONAL N.V, SWISS BRANCH AND/OR ITS AFFILIATED COMPANIES (collectively "STMICROELECTRONICS”), YOU (hereinafter referred also to as “THE RECIPIENT”), ON BEHALF OF YOURSELF, OR ON BEHALF OF ANY ENTITY BY WHICH YOU ARE EMPLOYED AND/OR ENGAGED, AGREE TO BE BOUND BY THIS AGREEMENT.

Under STMICROELECTRONICS’ intellectual property rights and subject to  applicable licensing terms for any third-party software incorporated in the SOFTWARE PACKAGE and applicable Open Source Terms (as defined here below), the redistribution, reproduction and use in source and binary forms of the SOFTWARE PACKAGE or any part thereof, with or without modification, are permitted provided that the following conditions are met:

1. Redistribution of source code (modified or not) must retain any copyright notice accompanying the SOFTWARE PACKAGE, this list of conditions and the disclaimer below.

2. Redistributions in binary form, except as embedded into a processing unit device manufactured by or for STMicroelectronics or a software update for any such device, must reproduce the accompanying copyright notice, this list of conditions and the below disclaimer in capital type, in the documentation and/or other materials provided with the distribution.

3. Neither the name of STMicroelectronics nor the names of other contributors to the SOFTWARE PACKAGE may be used to endorse or promote products derived from the SOFTWARE PACKAGE or part thereof without specific written permission.

4. The SOFTWARE PACKAGE or any part thereof, including modifications and/or derivative works of the SOFTWARE PACKAGE, must be used and execute solely and exclusively on or in combination with a processing unit device manufactured by or for STMicroelectronics.

5. No use, reproduction or redistribution of the SOFTWARE PACKAGE partially or totally may be done in any manner that would subject the SOFTWARE PACKAGE to any Open Source Terms (as defined below).

6. Some portion of the SOFTWARE PACKAGE may contain software subject to Open Source Terms (as defined below) applicable for each such portion (“Open Source Software”), as further specified in the SOFTWARE PACKAGE. Such Open Source Software is supplied under the applicable Open Source Terms and is not subject to the terms and conditions of this AGREEMENT. “Open Source Terms” shall mean any open source license which requires as part of distribution of software that the source code of such software is distributed therewith or otherwise made available, or open source license that substantially complies with the Open Source definition specified at www.opensource.org and any other comparable open source license such as for example GNU General Public License (GPL), Eclipse Public License (EPL), Apache Software License, BSD license and MIT license.

7. The SOFTWARE PACKAGE may also include third party software as expressly specified in the SOFTWARE PACKAGE subject to specific license terms from such third parties. Such third-party software is supplied under such specific license terms and is not subject to the terms and conditions of this AGREEMENT. By installing copying, downloading, accessing or otherwise using the SOFTWARE PACKAGE, the RECIPIENT agrees to be bound by such license terms with regard to such third-party software.

8. STMicroelectronics has no obligation to provide any maintenance, support or updates for the SOFTWARE PACKAGE.

9. The SOFTWARE PACKAGE is and will remain the exclusive property of STMicroelectronics and its licensors. The RECIPIENT will not take any action that jeopardizes STMicroelectronics and its licensors' proprietary rights or acquire any rights in the SOFTWARE PACKAGE, except the limited rights specified hereunder.

10. The RECIPIENT shall comply with all applicable laws and regulations affecting the use of the SOFTWARE PACKAGE or any part thereof including any applicable export control law or regulation.

11. Redistribution and use of the SOFTWARE PACKAGE partially or any part thereof other than as permitted under this AGREEMENT is void and will automatically terminate RECIPIENT’s rights under this AGREEMENT.

12. The RECIPIENT shall be solely liable to determine and verify that the SOFTWARE PACKAGE is fit for the RECIPIENT intended use, environment or application and comply with all regulatory, safety and security related requirements concerning any use.

DISCLAIMER:

THE SOFTWARE PACKAGE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THE SOFTWARE PACKAGE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
EXCEPT AS EXPRESSLY PERMITTED HEREUNDER AND SUBJECT TO THE APPLICABLE LICENSING TERMS FOR ANY THIRD-PARTY SOFTWARE INCORPORATED IN THE SOFTWARE PACKAGE AND OPEN SOURCE TERMS AS APPLICABLE, NO LICENSE OR OTHER RIGHTS, WHETHER EXPRESS OR IMPLIED, ARE GRANTED UNDER ANY PATENT OR OTHER INTELLECTUAL PROPERTY RIGHTS OF STMICROELECTRONICS OR ANY THIRD PARTY.

</div>


###### <a name="lic1"></a>

<label for="collapse-annex1" aria-hidden="true">__Annex 1__</label>
<div>
<div>

#### SLA0044 Rev6/October 2025

Software license agreement

ULTIMATE LIBERTY SOFTWARE LICENSE AGREEMENT

BY CLICKING ON THE "I ACCEPT" BUTTON OR BY UNZIPPING, INSTALLING, COPYING,
DOWNLOADING, ACCESSING OR OTHERWISE USING THIS SOFTWARE OR ANY PART THEREOF,
INCLUDING ANY RELATED DOCUMENTATION (collectively the "SOFTWARE")
FROM STMICROELECTRONICS INTERNATIONAL N.V, SWISS BRANCH AND/OR
ITS AFFILIATED COMPANIES (collectively "STMICROELECTRONICS"),
YOU (hereinafter referred also to as "THE RECIPIENT"), ON BEHALF OF YOURSELF,
OR ON BEHALF OF ANY ENTITY BY WHICH YOU ARE EMPLOYED AND/OR ENGAGED,
AGREE TO BE BOUND BY THIS AGREEMENT.

Under STMICROELECTRONICS' intellectual property rights, the redistribution,
reproduction and use in source and binary forms of the SOFTWARE or any part
thereof, with or without modification, are permitted provided that the following
conditions are met:

1. Redistribution of source code (modified or not) must retain any copyright
notice accompanying the SOFTWARE, this list of conditions and the disclaimer below.

2. Redistributions in binary form, except as embedded into a processing unit device
manufactured by or for STMicroelectronics or a software update for any such device,
must reproduce the accompanying copyright notice, this list of conditions,
and the below disclaimer in capital type, in the documentation and/or
other materials provided with the distribution.

3. Neither the name of STMicroelectronics nor the names of other contributors
to the SOFTWARE may be used to endorse or promote products derived
from the SOFTWARE or part thereof without specific written permission.

4. The SOFTWARE or any part thereof, including modifications and/or
derivative works of the SOFTWARE, must be used and execute solely
and exclusively on or in combination with a processing unit device
manufactured by or for STMicroelectronics.

5. No use, reproduction or redistribution of the SOFTWARE partially
or totally may be done in any manner that would subject the SOFTWARE
to any Open Source Terms. "Open Source Terms"  shall mean
any open source license which requires as part of distribution
of software that the source code of such software is distributed
therewith or otherwise made available, or open source license
that substantially complies with the Open Source definition specified
at www.opensource.org and any other comparable open source license
such as for example GNU General Public License (GPL),
Eclipse Public License (EPL), Apache Software License, BSD license
or MIT license.

6. STMicroelectronics has no obligation to provide any maintenance,
support or updates for the SOFTWARE.

7. The SOFTWARE is and will remain the exclusive property of
STMicroelectronics and its licensors. The RECIPIENT will not take
any action that jeopardizesBY CLICKING ON THE "I ACCEPT" BUTTON OR BY UNZIPPING, INSTALLING, COPYING,
DOWNLOADING, ACCESSING OR OTHERWISE USING THIS SOFTWARE OR ANY PART THEREOF,
INCLUDING ANY RELATED DOCUMENTATION (collectively the "SOFTWARE")
FROM STMICROELECTRONICS INTERNATIONAL N.V, SWISS BRANCH AND/OR
ITS AFFILIATED COMPANIES (collectively "STMICROELECTRONICS"),
YOU (hereinafter referred also to as "THE RECIPIENT"), ON BEHALF OF YOURSELF,
OR ON BEHALF OF ANY ENTITY BY WHICH YOU ARE EMPLOYED AND/OR ENGAGED,
AGREE TO BE BOUND BY THIS AGREEMENT.

Under STMICROELECTRONICS' intellectual property rights, the redistribution,
reproduction and use in source and binary forms of the SOFTWARE or any part
thereof, with or without modification, are permitted provided that the following
conditions are met:

1. Redistribution of source code (modified or not) must retain any copyright
notice accompanying the SOFTWARE, this list of conditions and the disclaimer below.

2. Redistributions in binary form, except as embedded into a processing unit device
manufactured by or for STMicroelectronics or a software update for any such device,
must reproduce the accompanying copyright notice, this list of conditions,
and the below disclaimer in capital type, in the documentation and/or
other materials provided with the distribution.

3. Neither the name of STMicroelectronics nor the names of other contributors
to the SOFTWARE may be used to endorse or promote products derived
from the SOFTWARE or part thereof without specific written permission.

4. The SOFTWARE or any part thereof, including modifications and/or
derivative works of the SOFTWARE, must be used and execute solely
and exclusively on or in combination with a processing unit device
manufactured by or for STMicroelectronics.

5. No use, reproduction or redistribution of the SOFTWARE partially
or totally may be done in any manner that would subject the SOFTWARE
to any Open Source Terms. "Open Source Terms"  shall mean
any open source license which requires as part of distribution
of software that the source code of such software is distributed
therewith or otherwise made available, or open source license
that substantially complies with the Open Source definition specified
at www.opensource.org and any other comparable open source license
such as for example GNU General Public License (GPL),
Eclipse Public License (EPL), Apache Software License, BSD license
or MIT license.

6. STMicroelectronics has no obligation to provide any maintenance,
support or updates for the SOFTWARE.

7. The SOFTWARE is and will remain the exclusive property of
STMicroelectronics and its licensors. The RECIPIENT will not take
any action that jeopardizes STMicroelectronics and its
licensors' proprietary rights or acquire any rights in the SOFTWARE,
except the limited rights specified hereunder.

8. The RECIPIENT shall comply with all applicable laws and regulations
affecting the use of the SOFTWARE or any part thereof including
any applicable export control law or regulation.

9. Redistribution and use of the SOFTWARE or any part thereof other
than as permitted under this AGREEMENT is void and will automatically
terminate RECIPIENT's rights under this AGREEMENT.

10. The RECIPIENT shall be solely liable to determine and verify that
the SOFTWARE is fit for the RECIPIENT intended use, environment or
application and comply with all regulatory, safety and security
related requirements concerning any use.

DISCLAIMER:

THE SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING,BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY RIGHTS, ARE
DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW.
IN NO EVENT SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THE
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
EXCEPT AS EXPRESSLY PERMITTED HEREUNDER, NO LICENSE OR OTHER RIGHTS,
WHETHER EXPRESS OR IMPLIED, ARE GRANTED UNDER ANY PATENT OR OTHER INTELLECTUAL
PROPERTY RIGHTS OF STMICROELECTRONICS OR ANY THIRD PARTY.

</div>
</div>


###### <a name="lic2"></a>
        ::: {.collapse}
        <input type="checkbox" id="collapse-annex2" checked aria-hidden="true">
        <label for="collapse-annex2" aria-hidden="true">__Annex 2__</label>
        <div>
        <div>
        
        #### BSD-3-Clause
        
        BSD-3-Clause

Copyright [YEAR] [COPYRIGHT HOLDER]

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

        
        </div>
        </div>
        :::
        
###### <a name="lic3"></a>
        ::: {.collapse}
        <input type="checkbox" id="collapse-annex3" checked aria-hidden="true">
        <label for="collapse-annex3" aria-hidden="true">__Annex 3__</label>
        <div>
        <div>
        
        #### Apache-2.0
        
        Apache-2.0

Version 2.0, February 2004
=========================

https://opensource.org/license/apache-2-0

TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION

1. Definitions.
“License” shall mean the terms and conditions for use, reproduction, and distribution as defined by Sections 1 through 9 of this document.

“Licensor” shall mean the copyright owner or entity authorized by the copyright owner that is granting the License.

“Legal Entity” shall mean the union of the acting entity and all other entities that control, are controlled by, or are under common control with that entity. For the purposes of this definition, “control” means (i) the power, direct or indirect, to cause the direction or management of such entity, whether by contract or otherwise, or (ii) ownership of fifty percent (50%) or more of the outstanding shares, or (iii) beneficial ownership of such entity.

“You” (or “Your”) shall mean an individual or Legal Entity exercising permissions granted by this License.

“Source” form shall mean the preferred form for making modifications, including but not limited to software source code, documentation source, and configuration files.

“Object” form shall mean any form resulting from mechanical transformation or translation of a Source form, including but not limited to compiled object code, generated documentation, and conversions to other media types.

“Work” shall mean the work of authorship, whether in Source or Object form, made available under the License, as indicated by a copyright notice that is included in or attached to the work (an example is provided in the Appendix below).

“Derivative Works” shall mean any work, whether in Source or Object form, that is based on (or derived from) the Work and for which the editorial revisions, annotations, elaborations, or other modifications represent, as a whole, an original work of authorship. For the purposes of this License, Derivative Works shall not include works that remain separable from, or merely link (or bind by name) to the interfaces of, the Work and Derivative Works thereof.

“Contribution” shall mean any work of authorship, including the original version of the Work and any modifications or additions to that Work or Derivative Works thereof, that is intentionally submitted to Licensor for inclusion in the Work by the copyright owner or by an individual or Legal Entity authorized to submit on behalf of the copyright owner. For the purposes of this definition, “submitted” means any form of electronic, verbal, or written communication sent to the Licensor or its representatives, including but not limited to communication on electronic mailing lists, source code control systems, and issue tracking systems that are managed by, or on behalf of, the Licensor for the purpose of discussing and improving the Work, but excluding communication that is conspicuously marked or otherwise designated in writing by the copyright owner as “Not a Contribution.”

“Contributor” shall mean Licensor and any individual or Legal Entity on behalf of whom a Contribution has been received by Licensor and subsequently incorporated within the Work.

2. Grant of Copyright License.
Subject to the terms and conditions of this License, each Contributor hereby grants to You a perpetual, worldwide, non-exclusive, no-charge, royalty-free, irrevocable copyright license to reproduce, prepare Derivative Works of, publicly display, publicly perform, sublicense, and distribute the Work and such Derivative Works in Source or Object form.

3. Grant of Patent License.
Subject to the terms and conditions of this License, each Contributor hereby grants to You a perpetual, worldwide, non-exclusive, no-charge, royalty-free, irrevocable (except as stated in this section) patent license to make, have made, use, offer to sell, sell, import, and otherwise transfer the Work, where such license applies only to those patent claims licensable by such Contributor that are necessarily infringed by their Contribution(s) alone or by combination of their Contribution(s) with the Work to which such Contribution(s) was submitted. If You institute patent litigation against any entity (including a cross-claim or counterclaim in a lawsuit) alleging that the Work or a Contribution incorporated within the Work constitutes direct or contributory patent infringement, then any patent licenses granted to You under this License for that Work shall terminate as of the date such litigation is filed.

4. Redistribution.
You may reproduce and distribute copies of the Work or Derivative Works thereof in any medium, with or without modifications, and in Source or Object form, provided that You meet the following conditions:

You must give any other recipients of the Work or Derivative Works a copy of this License; and
You must cause any modified files to carry prominent notices stating that You changed the files; and
You must retain, in the Source form of any Derivative Works that You distribute, all copyright, patent, trademark, and attribution notices from the Source form of the Work, excluding those notices that do not pertain to any part of the Derivative Works; and
If the Work includes a “NOTICE” text file as part of its distribution, then any Derivative Works that You distribute must include a readable copy of the attribution notices contained within such NOTICE file, excluding those notices that do not pertain to any part of the Derivative Works, in at least one of the following places: within a NOTICE text file distributed as part of the Derivative Works; within the Source form or documentation, if provided along with the Derivative Works; or, within a display generated by the Derivative Works, if and wherever such third-party notices normally appear. The contents of the NOTICE file are for informational purposes only and do not modify the License. You may add Your own attribution notices within Derivative Works that You distribute, alongside or as an addendum to the NOTICE text from the Work, provided that such additional attribution notices cannot be construed as modifying the License.
You may add Your own copyright statement to Your modifications and may provide additional or different license terms and conditions for use, reproduction, or distribution of Your modifications, or for any such Derivative Works as a whole, provided Your use, reproduction, and distribution of the Work otherwise complies with the conditions stated in this License.

5. Submission of Contributions.
Unless You explicitly state otherwise, any Contribution intentionally submitted for inclusion in the Work by You to the Licensor shall be under the terms and conditions of this License, without any additional terms or conditions. Notwithstanding the above, nothing herein shall supersede or modify the terms of any separate license agreement you may have executed with Licensor regarding such Contributions.

6. Trademarks.
This License does not grant permission to use the trade names, trademarks, service marks, or product names of the Licensor, except as required for reasonable and customary use in describing the origin of the Work and reproducing the content of the NOTICE file.

7. Disclaimer of Warranty.
Unless required by applicable law or agreed to in writing, Licensor provides the Work (and each Contributor provides its Contributions) on an “AS IS” BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied, including, without limitation, any warranties or conditions of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A PARTICULAR PURPOSE. You are solely responsible for determining the appropriateness of using or redistributing the Work and assume any risks associated with Your exercise of permissions under this License.

8. Limitation of Liability.
In no event and under no legal theory, whether in tort (including negligence), contract, or otherwise, unless required by applicable law (such as deliberate and grossly negligent acts) or agreed to in writing, shall any Contributor be liable to You for damages, including any direct, indirect, special, incidental, or consequential damages of any character arising as a result of this License or out of the use or inability to use the Work (including but not limited to damages for loss of goodwill, work stoppage, computer failure or malfunction, or any and all other commercial damages or losses), even if such Contributor has been advised of the possibility of such damages.

9. Accepting Warranty or Additional Liability.
While redistributing the Work or Derivative Works thereof, You may choose to offer, and charge a fee for, acceptance of support, warranty, indemnity, or other liability obligations and/or rights consistent with this License. However, in accepting such obligations, You may act only on Your own behalf and on Your sole responsibility, not on behalf of any other Contributor, and only if You agree to indemnify, defend, and hold each Contributor harmless for any liability incurred by, or claims asserted against, such Contributor by reason of your accepting any such warranty or additional liability.

END OF TERMS AND CONDITIONS

APPENDIX: How to apply the Apache License to your work
To apply the Apache License to your work, attach the following boilerplate notice, with the fields enclosed by brackets “[]” replaced with your own identifying information. (Don’t include the brackets!) The text should be enclosed in the appropriate comment syntax for the file format. We also recommend that a file or class name and description of purpose be included on the same “printed page” as the copyright notice for easier identification within third-party archives.

   Copyright [yyyy] [name of copyright owner]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

        
        </div>
        </div>
        :::
        
###### <a name="lic4"></a>
        ::: {.collapse}
        <input type="checkbox" id="collapse-annex4" checked aria-hidden="true">
        <label for="collapse-annex4" aria-hidden="true">__Annex 4__</label>
        <div>
        <div>
        
        #### MIT
        
        MIT

Copyright [YEAR] [COPYRIGHT HOLDER]

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

        
        </div>
        </div>
        :::
        
<label for="collapse-copyrights" aria-hidden="true">__Copyrights Statements__</label>
<div>
###### <a name="cp1"></a>

        <label for="collapse-copyright1" aria-hidden="true">Full Copyrights for stm32c5xx_DFP</label>
        
        <div>
        
        Copyrights statements for [stm32c5xx_DFP]

	Copyright (c) 2026 STMicroelectronics.
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp2"></a>

        <label for="collapse-copyright2" aria-hidden="true">Full Copyrights for stm32c5xx_HAL_Drivers</label>
        
        <div>
        
        Copyrights statements for [stm32c5xx_HAL_Drivers]

	Copyright (c) 2026 STMicroelectronics.
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp3"></a>

        <label for="collapse-copyright3" aria-hidden="true">Full Copyrights for button_Part_Drivers</label>
        
        <div>
        
        Copyrights statements for [button_Part_Drivers]

	Copyright (c) 2026 STMicroelectronics",
	
	Copyright (c) 2026 STMicroelectronics.
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp4"></a>

        <label for="collapse-copyright4" aria-hidden="true">Full Copyrights for lan8742_Part_Drivers</label>
        
        <div>
        
        Copyrights statements for [lan8742_Part_Drivers]

	Copyright (c) 2026 STMicroelectronics",
	
	Copyright (c) 2026 STMicroelectronics.
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp5"></a>

        <label for="collapse-copyright5" aria-hidden="true">Full Copyrights for led_Part_Drivers</label>
        
        <div>
        
        Copyrights statements for [led_Part_Drivers]

	Copyright (c) 2026 STMicroelectronics.
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp6"></a>

        <label for="collapse-copyright6" aria-hidden="true">Full Copyrights for w25n01gvxx_Part_Drivers</label>
        
        <div>
        
        Copyrights statements for [w25n01gvxx_Part_Drivers]

	Copyright (c) 2026 STMicroelectronics.
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp7"></a>

        <label for="collapse-copyright7" aria-hidden="true">Full Copyrights for w25q128j_Part_Drivers</label>
        
        <div>
        
        Copyrights statements for [w25q128j_Part_Drivers]

	Copyright (c) 2026 STMicroelectronics.
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp8"></a>

        <label for="collapse-copyright8" aria-hidden="true">Full Copyrights for ARM-software/CMSIS-DSP</label>
        
        <div>
        
        Copyrights statements for [ARM-software/CMSIS-DSP]

	Copyright (C) 1997-2020 by Dimitri van Heesch
	
	Copyright (c) 2007 Ariel Flesler - aflesler
	
	Copyright (c) 2010-2019 Arm Limited or its affiliates. All rights reserved
	
	Copyright (c) 2010-2020 Arm Limited or its affiliates. All rights reserved
	
	Copyright (c) 2010-2021 Arm Limited or its affiliates. All rights reserved
	
	Copyright (C) 2010-2022 ARM Limited or its affiliates. All rights reserved
	
	Copyright (c) 2018 Steven Benner (http://stevenbenner.com/).
	
	Copyright (C) 2019-2020 ARM Limited. All rights reserved
	
	Copyright (c) 2021 - 2022 jothepro
	
	Copyright jQuery Foundation and other contributors; Licensed MIT
	
	Copyright Vasil Dinkov, Vadikom Web Ltd. http://vadikom.com; Licensed MIT
	
	copyright</span></div>
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp9"></a>

        <label for="collapse-copyright9" aria-hidden="true">Full Copyrights for ARM-software/CMSIS_6</label>
        
        <div>
        
        Copyrights statements for [ARM-software/CMSIS_6]

	Copyright (C) 1997-2020 by Dimitri van Heesch
	Copyright (c) 2006-2016, Arm Limited, All Rights Reserved
	Copyright (c) 2007 Ariel Flesler - aflesler
	Copyright (c) 2009-2017 ARM Limited. All rights reserved
	Copyright (c) 2009-2018 Arm Limited. All rights reserved
	Copyright (c) 2009-2021 Arm Limited. All rights reserved
	Copyright (c) 2009-2022 Arm Limited. All rights reserved
	Copyright (c) 2009-2023 ARM Limited. All rights reserved
	Copyright (c) 2009-2024 Arm Limited. All rights reserved
	Copyright (c) 2009-2025 Arm Limited.
	Copyright (c) 2009-2025 Arm Limited. All rights reserved
	Copyright (c) 2013-2017 ARM Limited. All rights reserved
	Copyright (c) 2013-2018 Arm Limited. All rights reserved
	Copyright (c) 2013-2020 ARM Limited. All rights reserved
	Copyright (c) 2013-2023 Arm Limited. All rights reserved
	Copyright (c) 2013-2024 Arm Limited. All rights reserved
	Copyright (c) 2015-2020 ARM Limited. All rights reserved
	Copyright (c) 2016-2018 Arm Limited. All rights reserved
	Copyright (c) 2017 ARM Limited. All rights reserved
	Copyright (c) 2017-2018 Arm Limited. All rights reserved
	Copyright (c) 2017-2020 ARM Limited. All rights reserved
	Copyright (c) 2017-2021 ARM Limited. All rights reserved
	Copyright (c) 2017-2022 ARM Limited. All rights reserved
	Copyright (c) 2017-2023 ARM Limited. All rights reserved
	Copyright (c) 2017-2024 Arm Limited. All rights reserved
	Copyright (c) 2017-2024 IAR Systems
	Copyright (c) 2018 Steven Benner (http://stevenbenner.com/).
	Copyright (c) 2018-2022 Arm China.
	Copyright (c) 2018-2023 Arm Limited. All rights reserved
	Copyright (c) 2018-2024 Arm Limited. All rights reserved
	Copyright (c) 2018-2025 Arm Limited. All rights reserved
	Copyright (c) 2018-2025 Arm Limited. Copyright (c) 2024 Arm Technology (China) Co., Ltd. All rights reserved
	Copyright (c) 2018-2025 Arm Limited. Copyright (c) 2025 Arm Technology (China) Co., Ltd. All rights reserved
	Copyright (c) 2019-2022 Arm Limited. All rights reserved
	Copyright (c) 2019-2023 Arm Limited. All rights reserved
	Copyright (c) 2020 Arm Limited. All rights reserved
	Copyright (c) 2020-2021 Arm Limited. All rights reserved
	Copyright (c) 2021 - 2022 jothepro
	Copyright (c) 2022 Arm Limited. All rights reserved
	Copyright (c) 2022-2024 Arm Limited. All rights reserved
	Copyright (c) 2023 ARM Limited. All rights reserved
	Copyright (c) 2023-2024 Arm Limited. All rights reserved
	Copyright (c) 2023-2025 ARM Limited. All rights reserved
	Copyright (c) 2025 Arm Limited. All rights reserved
	Copyright jQuery Foundation and other contributors; Licensed MIT
	Copyright Vasil Dinkov, Vadikom Web Ltd. http://vadikom.com; Licensed MIT
	Copyright © 2026 Arm Limited (or its affiliates). All rights reserved
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp10"></a>

        <label for="collapse-copyright10" aria-hidden="true">Full Copyrights for Advanced_Trace</label>
        
        <div>
        
        Copyrights statements for [Advanced_Trace]

    Copyright (c) 2024-2026 STMicroelectronics.

    Copyright (c) 2026 STMicroelectronics.
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp11"></a>

        <label for="collapse-copyright11" aria-hidden="true">Full Copyrights for Basic_STDIO</label>
        
        <div>
        
        Copyrights statements for [Basic_STDIO]

    Copyright (c) 2026 STMicroelectronics.
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp12"></a>

        <label for="collapse-copyright12" aria-hidden="true">Full Copyrights for EEPROM_Emulation</label>
        
        <div>
        
        Copyrights statements for [EEPROM_Emulation]

    Copyright (c) 2026 STMicroelectronics.
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp13"></a>

        <label for="collapse-copyright13" aria-hidden="true">Full Copyrights for Sequencer</label>
        
        <div>
        
        Copyrights statements for [Sequencer]

    Copyright (c) 2019-2026 STMicroelectronics.

    Copyright (c) 2026 STMicroelectronics.
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp14"></a>

        <label for="collapse-copyright14" aria-hidden="true">Full Copyrights for Syscalls</label>
        
        <div>
        
        Copyrights statements for [Syscalls]

    Copyright (c) 2026 STMicroelectronics.
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp15"></a>

        <label for="collapse-copyright15" aria-hidden="true">Full Copyrights for FreeRTOS Real Time Kernel</label>
        
        <div>
        
        Copyrights statements for [FreeRTOS Real Time Kernel]

	Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved
	
	Copyright 2024 Arm Limited and/or its affiliates
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp16"></a>

        <label for="collapse-copyright16" aria-hidden="true">Full Copyrights for MbedTLS</label>
        
        <div>
        
        Copyrights statements for [MbedTLS]

	Copyright (C) STMicroelectronics, All Rights Reserved
	
	Copyright The Mbed TLS Contributors
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp17"></a>

        <label for="collapse-copyright17" aria-hidden="true">Full Copyrights for STCryptoLib</label>
        
        <div>
        
        Copyrights statements for [STCryptoLib]

	Copyright (c) 2021-2026 STMicroelectronics.
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp18"></a>

        <label for="collapse-copyright18" aria-hidden="true">Full Copyrights for STFCF</label>
        
        <div>
        
        Copyrights statements for [STFCF]

	Copyright (c) 2026 STMicroelectronics - All rights reserved
	
	Copyright (c) 2026 STMicroelectronics.
	
	Copyright The Mbed TLS Contributors
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp19"></a>

        <label for="collapse-copyright19" aria-hidden="true">Full Copyrights for eclipse-threadx/filex</label>
        
        <div>
        
        Copyrights statements for [eclipse-threadx/filex]

	Copyright (c) 2024 Microsoft Corporation
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp20"></a>

        <label for="collapse-copyright20" aria-hidden="true">Full Copyrights for eclipse-threadx/levelx</label>
        
        <div>
        
        Copyrights statements for [eclipse-threadx/levelx]

	Copyright (c) 2024 Microsoft Corporation
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp21"></a>

        <label for="collapse-copyright21" aria-hidden="true">Full Copyrights for eclipse-threadx/usbx_STM32</label>
        
        <div>
        
        Copyrights statements for [eclipse-threadx/usbx_STM32]

	Copyright (c) 2024 Microsoft Corporation.

	Copyright (c) 2025-2026 STMicroelectronics.

	Copyright (c) 2026 STMicroelectronics.

	Copyright (c) Microsoft Corporation. All rights reserved
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp22"></a>

        <label for="collapse-copyright22" aria-hidden="true">Full Copyrights for git.trustedfirmware.org/TF-M/trusted-firmware-m</label>
        
        <div>
        
        Copyrights statements for [git.trustedfirmware.org/TF-M/trusted-firmware-m]

	Copyright (c) 2017 Linaro Limited
	
	Copyright (c) 2018-2020 Arm Limited.
	
	Copyright (c) 2018-2020, Arm Limited. All rights reserved
	
	Copyright (c) 2019 Arm Limited.
	
	Copyright (c) 2020-2023, Arm Limited. All rights reserved
	
	Copyright (c) 2021-2024, Arm Limited. All rights reserved
	
	git.trustedfirmware.org/TF-M/trusted-firmware-m 1.3.0 long_tail:git.trustedfirmware.org/TF-M/trusted-firmware-m#TF-Mv1.3.0
	
	Copyright (c) 2021-2024, Arm Limited. All rights reserved
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp23"></a>

        <label for="collapse-copyright23" aria-hidden="true">Full Copyrights for lwIP</label>
        
        <div>
        
        Copyrights statements for [lwIP]

	Copyright (c) 1984-2000 Carnegie Mellon University. All rights reserved
	
	Copyright (c) 1989 Carnegie Mellon University.
	
	Copyright (c) 1989 Regents of the University of California.
	
	Copyright (c) 1993-2002 Paul Mackerras. All rights reserved
	
	Copyright (c) 1994 The Australian National University.
	
	Copyright (c) 1994-2002 Paul Mackerras. All rights reserved
	
	Copyright (c) 1995 Eric Rosenquist. All rights reserved
	
	Copyright (c) 1995, 1996, 1997 Francis.Dupont@inria.fr, INRIA Rocquencourt,
	
	Copyright (c) 1996-2002 Paul Mackerras. All rights reserved
	
	Copyright (c) 1997 by Global Election Systems Inc.
	
	Copyright (c) 1998 by Global Election Systems Inc.
	
	Copyright (c) 1998, 1999 Francis.Dupont@inria.fr, GIE DYADE,
	
	Copyright (c) 1999 Tommi Komulainen. All rights reserved
	
	Copyright (c) 1999-2002 Paul Mackerras. All rights reserved
	
	Copyright (c) 2000-2002 Paul Mackerras. All rights reserved
	
	Copyright (c) 2001 by Sun Microsystems, Inc.
	
	Copyright (c) 2001-2003 Swedish Institute of Computer Science.
	
	Copyright (c) 2001-2004 Axon Digital Design B.V., The Netherlands.
	
	Copyright (c) 2001-2004 Leon Woestenberg <leon.woestenberg@gmx.net>
	
	Copyright (c) 2001-2004 Swedish Institute of Computer Science.
	
	Copyright (c) 2002 CITEL Technologies Ltd.
	
	Copyright (c) 2002 Google, Inc.
	
	Copyright (c) 2002 Google, Inc. All rights reserved
	
	Copyright (c) 2002 The NetBSD Foundation, Inc.
	
	Copyright (c) 2002,2003,2004 Google, Inc.
	
	Copyright (c) 2002-2003, Adam Dunkels.
	
	Copyright (c) 2003 by Marc Boucher, Services Informatiques (MBSI) inc.
	
	Copyright (c) 2003 Paul Mackerras. All rights reserved
	
	Copyright (c) 2003-2004 Axon Digital Design B.V., The Netherlands.
	
	Copyright (c) 2003-2004 Leon Woestenberg <leon.woestenberg@axon.tv>
	
	Copyright (c) 2006 by Marc Boucher, Services Informatiques (MBSI) inc.
	
	Copyright (C) 2006-2008 Christophe Devine
	
	Copyright (c) 2007 Dominik Spies <kontakt@dspies.de>
	
	Copyright (c) 2007-2009 Fr
	
	Copyright (C) 2009 Paul Bakker <polarssl_maintainer at polarssl dot org>
	
	Copyright (c) 2010 Inico Technologies Ltd.
	
	Copyright (c) 2014 Simon Goldschmidt
	
	Copyright (c) 2015 Inico Technologies Ltd.
	
	Copyright (c) 2015 Inico Technologies Ltd. , Author: Ivan Delamer <delamer@inicotech.com>
	
	Copyright (c) 2015 Verisure Innovation AB
	
	Copyright (c) 2016 Erik Andersson <erian747@gmail.com>
	
	Copyright (c) 2017 Benjamin Aigner
	
	Copyright (c) 2017 Simon Goldschmidt
	
	Copyright (c) 2017 Simon Goldschmidt.
	
	Copyright (c) 2018 Jasper Verschueren <jasper.verschueren@apart-audio.com>
	
	Copyright (c) 2018 Simon Goldschmidt
	
	Copyright (c) 2026 STMicroelectronics.
	
	Copyright (c) Deltatee Enterprises Ltd. 2013
	
	lwIP STABLE-2_2_1_RELEASE gnu:lwip:STABLE-2_2_1_RELEASE: http://savannah.nongnu.org/projects/lwip/
	
	Copyright (c) 1984-2000 Carnegie Mellon University. All rights reserved
	
	Copyright (c) 1989 Carnegie Mellon University.
	
	Copyright (c) 1989 Regents of the University of California.
	
	Copyright (c) 1993-2002 Paul Mackerras. All rights reserved
	
	Copyright (c) 1994 The Australian National University.
	
	Copyright (c) 1994-2002 Paul Mackerras. All rights reserved
	
	Copyright (c) 1995 Eric Rosenquist. All rights reserved
	
	Copyright (c) 1995, 1996, 1997 Francis.Dupont@inria.fr, INRIA Rocquencourt,
	
	Copyright (c) 1996-2002 Paul Mackerras. All rights reserved
	
	Copyright (c) 1997 by Global Election Systems Inc.
	
	Copyright (c) 1997 Global Election Systems Inc.
	
	Copyright (c) 1998 by Global Election Systems Inc.
	
	Copyright (c) 1998 Global Election Systems Inc.
	
	Copyright (c) 1998, 1999 Francis.Dupont@inria.fr, GIE DYADE,
	
	Copyright (c) 1999 Tommi Komulainen. All rights reserved
	
	Copyright (c) 1999-2002 Paul Mackerras. All rights reserved
	
	Copyright (c) 2000-2002 Paul Mackerras. All rights reserved
	
	Copyright (c) 2001 by Cognizant Pty Ltd.
	
	Copyright (c) 2001 by Sun Microsystems, Inc.
	
	Copyright (c) 2001, 2002 Axon Digital Design B.V., The Netherlands.
	
	Copyright (c) 2001, 2002 Leon Woestenberg <leon.woestenberg@axon.tv>
	
	Copyright (c) 2001, Swedish Institute of Computer Science.
	
	Copyright (c) 2001-2003 Swedish Institute of Computer Science.
	
	Copyright (c) 2001-2004 Axon Digital Design B.V., The Netherlands.
	
	Copyright (c) 2001-2004 Leon Woestenberg <leon.woestenberg@gmx.net>
	
	Copyright (c) 2001-2004 Swedish Institute of Computer Science.
	
	Copyright (c) 2002 CITEL Technologies Ltd.
	
	Copyright (c) 2002 Google, Inc.
	
	Copyright (c) 2002 Google, Inc. All rights reserved
	
	Copyright (c) 2002 The NetBSD Foundation, Inc.
	
	Copyright (c) 2002,2003,2004 Google, Inc.
	
	Copyright (c) 2002-2003, Adam Dunkels.
	
	Copyright (c) 2003 by Marc Boucher, Services Informatiques (MBSI) inc.
	
	Copyright (c) 2003 Paul Mackerras. All rights reserved
	
	Copyright (c) 2003-2004 Axon Digital Design B.V., The Netherlands.
	
	Copyright (c) 2003-2004 Leon Woestenberg <leon.woestenberg@axon.tv>
	
	Copyright (c) 2006 Axon Digital Design B.V., The Netherlands.
	
	Copyright (c) 2006 by Marc Boucher, Services Informatiques (MBSI) inc.
	
	Copyright (C) 2006-2008 Christophe Devine
	
	Copyright (c) 2007 Dominik Spies <kontakt@dspies.de>
	
	Copyright (c) 2007-2009 Fr
	
	Copyright (c) 2008 Paul Mackerras. All rights reserved
	
	Copyright (C) 2009 Paul Bakker <polarssl_maintainer at polarssl dot org>
	
	Copyright (c) 2010 Inico Technologies Ltd.
	
	Copyright (c) 2014 Simon Goldschmidt
	
	Copyright (c) 2015 Dirk Ziegelmeier
	
	Copyright (c) 2015 Inico Technologies Ltd.
	
	Copyright (c) 2015 Inico Technologies Ltd. , Author: Ivan Delamer <delamer@inicotech.com>
	
	Copyright (c) 2015 Verisure Innovation AB
	
	Copyright (c) 2016 Elias Oenal and Dirk Ziegelmeier.
	
	Copyright (c) 2016 Elias Oenal.
	
	Copyright (c) 2016 Erik Andersson
	
	Copyright (c) 2016 Erik Andersson <erian747@gmail.com>
	
	Copyright (c) 2017 Benjamin Aigner
	
	Copyright (c) 2017 Dirk Ziegelmeier.
	
	Copyright (c) 2017 Joel Cunningham, Garmin International, Inc. <joel.cunningham@garmin.com>
	
	Copyright (c) 2017 Simon Goldschmidt
	
	Copyright (c) 2017 Simon Goldschmidt <goldsimon@gmx.de>
	
	Copyright (c) 2017 Simon Goldschmidt.
	
	Copyright (c) 2017 The MINIX 3 Project.
	
	Copyright (c) 2018 Jasper Verschueren <jasper.verschueren@apart-audio.com>
	
	Copyright (c) 2018 Simon Goldschmidt
	
	Copyright (c) 2018 Simon Goldschmidt <goldsimon@gmx.de>
	
	Copyright (c) 2018 Simon Goldschmidt.
	
	Copyright (c) 2018 Swedish Institute of Computer Science.
	
	Copyright (c) Deltatee Enterprises Ltd. 2013
        
        </div>
        
        [Back to top](#top)
        <BR>
        
###### <a name="cp24"></a>

        <label for="collapse-copyright24" aria-hidden="true">Full Copyrights for RoT_Provisioning</label>
        
        <div>
        
        Copyrights statements for [RoT_Provisioning]

	Copyright (c) 2026 STMicroelectronics - All rights reserved
	
	Copyright (c) 2026 STMicroelectronics.
        
        </div>
        
        [Back to top](#top)
        <BR>
        
<div>

</div>
</div>

