


/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/

/********************************************************************
-usb_descriptors.c-
-------------------------------------------------------------------
Filling in the descriptor values in the usb_descriptors.c file:
-------------------------------------------------------------------

[Device Descriptors]
The device descriptor is defined as a USB_DEVICE_DESCRIPTOR type.  
This type is defined in usb_ch9.h  Each entry into this structure
needs to be the correct length for the data type of the entry.

[Configuration Descriptors]
The configuration descriptor was changed in v2.x from a structure
to a uint8_t array.  Given that the configuration is now a byte array
each byte of multi-byte fields must be listed individually.  This
means that for fields like the total size of the configuration where
the field is a 16-bit value "64,0," is the correct entry for a
configuration that is only 64 bytes long and not "64," which is one
too few bytes.

The configuration attribute must always have the _DEFAULT
definition at the minimum. Additional options can be ORed
to the _DEFAULT attribute. Available options are _SELF and _RWU.
These definitions are defined in the usb_device.h file. The
_SELF tells the USB host that this device is self-powered. The
_RWU tells the USB host that this device supports Remote Wakeup.

[Endpoint Descriptors]
Like the configuration descriptor, the endpoint descriptors were 
changed in v2.x of the stack from a structure to a uint8_t array.  As
endpoint descriptors also has a field that are multi-byte entities,
please be sure to specify both bytes of the field.  For example, for
the endpoint size an endpoint that is 64 bytes needs to have the size
defined as "64,0," instead of "64,"

Take the following example:
    // Endpoint Descriptor //
    0x07,                       //the size of this descriptor //
    USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
    _EP02_IN,                   //EndpointAddress
    _INT,                       //Attributes
    0x08,0x00,                  //size (note: 2 bytes)
    0x02,                       //Interval

The first two parameters are self-explanatory. They specify the
length of this endpoint descriptor (7) and the descriptor type.
The next parameter identifies the endpoint, the definitions are
defined in usb_device.h and has the following naming
convention:
_EP<##>_<dir>
where ## is the endpoint number and dir is the direction of
transfer. The dir has the value of either 'OUT' or 'IN'.
The next parameter identifies the type of the endpoint. Available
options are _BULK, _INT, _ISO, and _CTRL. The _CTRL is not
typically used because the default control transfer endpoint is
not defined in the USB descriptors. When _ISO option is used,
addition options can be ORed to _ISO. Example:
_ISO|_AD|_FE
This describes the endpoint as an isochronous pipe with adaptive
and feedback attributes. See usb_device.h and the USB
specification for details. The next parameter defines the size of
the endpoint. The last parameter in the polling interval.

-------------------------------------------------------------------
Adding a USB String
-------------------------------------------------------------------
A string descriptor array should have the following format:

rom struct{byte bLength;byte bDscType;word string[size];}sdxxx={
sizeof(sdxxx),DSC_STR,<text>};

The above structure provides a means for the C compiler to
calculate the length of string descriptor sdxxx, where xxx is the
index number. The first two bytes of the descriptor are descriptor
length and type. The rest <text> are string texts which must be
in the unicode format. The unicode format is achieved by declaring
each character as a word type. The whole text string is declared
as a word array with the number of characters equals to <size>.
<size> has to be manually counted and entered into the array
declaration. Let's study this through an example:
if the string is "USB" , then the string descriptor should be:
(Using index 02)
rom struct{byte bLength;byte bDscType;word string[3];}sd002={
sizeof(sd002),DSC_STR,'U','S','B'};

A USB project may have multiple strings and the firmware supports
the management of multiple strings through a look-up table.
The look-up table is defined as:
rom const unsigned char *rom USB_SD_Ptr[]={&sd000,&sd001,&sd002};

The above declaration has 3 strings, sd000, sd001, and sd002.
Strings can be removed or added. sd000 is a specialized string
descriptor. It defines the language code, usually this is
US English (0x0409). The index of the string must match the index
position of the USB_SD_Ptr array, &sd000 must be in position
USB_SD_Ptr[0], &sd001 must be in position USB_SD_Ptr[1] and so on.
The look-up table USB_SD_Ptr is used by the get string handler
function.

-------------------------------------------------------------------

The look-up table scheme also applies to the configuration
descriptor. A USB device may have multiple configuration
descriptors, i.e. CFG01, CFG02, etc. To add a configuration
descriptor, user must implement a structure similar to CFG01.
The next step is to add the configuration descriptor name, i.e.
cfg01, cfg02,.., to the look-up table USB_CD_Ptr. USB_CD_Ptr[0]
is a dummy place holder since configuration 0 is the un-configured
state according to the definition in the USB specification.

********************************************************************/
 
/*********************************************************************
 * Descriptor specific type definitions are defined in:
 * usb_device.h
 *
 * Configuration options are defined in:
 * usb_device_config.h
 ********************************************************************/
#ifndef __USB_DESCRIPTORS_C
#define __USB_DESCRIPTORS_C
 
/** INCLUDES *******************************************************/
#include "usb.h"
#include "usb_device_generic.h"

/** CONSTANTS ******************************************************/
#if defined(__18CXX)
#pragma romdata
#endif

/* Device Descriptor */
const USB_DEVICE_DESCRIPTOR device_dsc=
{
    0x12,                   // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,  // DEVICE descriptor type
    0x0200,                 // USB Spec Release Number in BCD format
    0x00,                   // Class Code
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    USB_EP0_BUFF_SIZE,      // Max packet size for EP0, see usb_device_config.h
    0x04D8,                 // Vendor ID
    0x0053,                 // Product ID
    0x0100,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x03,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
const uint8_t configDescriptor1[]={
    /* Configuration Descriptor */
    0x09,//sizeof(USB_CFG_DSC),    // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,  // CONFIGURATION descriptor type
    0x20,0x00,                     // Total length of data for this cfg
    1,                             // Number of interfaces in this cfg
    1,                             // Index value of this configuration
    0,                             // Configuration string index
    _DEFAULT | _SELF,              // Attributes, see usb_device.h
    50,                            // Max power consumption (2X mA)

    /* Interface Descriptor */
    0x09,//sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,      // INTERFACE descriptor type
    0,                             // Interface Number
    0,                             // Alternate Setting Number
    2,                             // Number of endpoints in this intf
    0xFF,                          // Class code
    0xFF,                          // Subclass code
    0xFF,                          // Protocol code
    0,                             // Interface string index

    /* Endpoint Descriptor */
    0x07,                       /*sizeof(USB_EP_DSC)*/
    USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
    _EP01_OUT,                  //EndpointAddress
    _INTERRUPT,                 //Attributes
    USBGEN_EP_SIZE,0x00,        //size
    10,                          //Interval

    0x07,                       /*sizeof(USB_EP_DSC)*/
    USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
    _EP01_IN,                   //EndpointAddress
    _INTERRUPT,                 //Attributes
    USBGEN_EP_SIZE,0x00,        //size
    10                           //Interval
};

//Language code string descriptor
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[1];}sd000={
sizeof(sd000),USB_DESCRIPTOR_STRING,{0x0409}};

//Manufacturer string descriptor
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[25];}sd001={
sizeof(sd001),USB_DESCRIPTOR_STRING,
{'M','i','c','r','o','c','h','i','p',' ','T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'}
};

//Product string descriptor
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[10];}sd002={
sizeof(sd002),USB_DESCRIPTOR_STRING,
{'S','m','a','r','t',' ','S','h','o','e'}
};

//Serial number string descriptor.  If a serial number string is implemented, 
//it should be unique for every single device coming off the production assembly 
//line.  Plugging two devices with the same serial number into a computer 
//simultaneously will cause problems (in extreme cases BSOD).
//Note: Common OSes put restrictions on the possible values that are allowed.
//For best OS compatibility, the serial number string should only consist
//of UNICODE encoded numbers 0 through 9 and capital letters A through F.
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[3];}sd003={
sizeof(sd003),USB_DESCRIPTOR_STRING,
{'1','2','4'}
};

//Array of configuration descriptors
const uint8_t *const USB_CD_Ptr[]=
{
    (const uint8_t *const)&configDescriptor1
};

//Array of string descriptors
const uint8_t *const USB_SD_Ptr[USB_NUM_STRING_DESCRIPTORS]=
{
    (const uint8_t *const)&sd000,
    (const uint8_t *const)&sd001,
    (const uint8_t *const)&sd002,
    (const uint8_t *const)&sd003
};

#if defined(IMPLEMENT_MICROSOFT_OS_DESCRIPTOR)
    //Microsoft "OS Descriptor" - This descriptor is based on a Microsoft specific 
    //specification (not part of the standard USB 2.0 specs or class specs). 
    //Implementing this special descriptor allows WinUSB driver package installation
    //to be automatic on Windows 8.  For additional details, see:
    //http://msdn.microsoft.com/en-us/library/windows/hardware/hh450799(v=vs.85).aspx
    const MS_OS_DESCRIPTOR MSOSDescriptor =
    {   
        sizeof(MSOSDescriptor),         //bLength - lenght of this descriptor in bytes
        USB_DESCRIPTOR_STRING,          //bDescriptorType - "string"
        {'M','S','F','T','1','0','0'},  //qwSignature - special values that specifies the OS descriptor spec version that this firmware implements
        GET_MS_DESCRIPTOR,              //bMS_VendorCode - defines the "GET_MS_DESCRIPTOR" bRequest literal value
        0x00                            //bPad - always 0x00
    };
    
    
    //Extended Compat ID OS Feature Descriptor
    const MS_COMPAT_ID_FEATURE_DESC CompatIDFeatureDescriptor =
    {
        //----------Header Section--------------
        sizeof(CompatIDFeatureDescriptor),  //dwLength
        0x0100,                             //bcdVersion = 1.00
        EXTENDED_COMPAT_ID,                 //wIndex
        0x01,                               //bCount - 0x01 "Function Section(s)" implemented in this descriptor
        {0,0,0,0,0,0,0},                    //Reserved[7]
        //----------Function Section 1----------
        0x00,                               //bFirstInterfaceNumber: the WinUSB interface in this firmware is interface #0
        0x01,                               //Reserved - fill this reserved byte with 0x01 according to documentation
        {'W','I','N','U','S','B',0x00,0x00},//compatID - "WINUSB" (with two null terminators to fill all 8 bytes)
        {0,0,0,0,0,0,0,0},                  //subCompatID - eight bytes of 0
        {0,0,0,0,0,0}                       //Reserved
    };    
    
    
    //Extended Properties OS Feature Descriptor
    const MS_EXT_PROPERTY_FEATURE_DESC ExtPropertyFeatureDescriptor =
    {
        //----------Header Section--------------
        sizeof(ExtPropertyFeatureDescriptor),   //dwLength
        0x0100,                                 //bcdVersion = 1.00
        EXTENDED_PROPERTIES,                    //wIndex
        0x0001,                                 //wCount - 0x0001 "Property Sections" implemented in this descriptor
        //----------Property Section 1----------
        132,                                    //dwSize - 132 bytes in this Property Section
        0x00000001,                             //dwPropertyDataType (Unicode string)
        40,                                     //wPropertyNameLength - 40 bytes in the bPropertyName field
        {'D','e','v','i','c','e','I','n','t','e','r','f','a','c','e','G','U','I','D', 0x0000},  //bPropertyName - "DeviceInterfaceGUID"
        78,                                     //dwPropertyDataLength - 78 bytes in the bPropertyData field (GUID value in UNICODE formatted string, with braces and dashes)
        //The below value is the Device Interface GUID (a 128-bit long "globally unique identifier")
        //Please modify the GUID value in your application before moving to production.
        //When you change the GUID, you must also change the PC application software
        //that connects to this device, as the software looks for the device based on 
        //VID, PID, and GUID.  All three values in the PC application must match 
        //the values in this firmware.
        //The GUID value can be a randomly generated 128-bit hexadecimal number, 
        //formatted like the below value.  The actual value is not important,
        //so long as it is almost certain to be globally unique, and matches the
        //PC software that communicates with this firmware.
        {'{','5','8','d','0','7','2','1','0','-','2','7','c','1','-','1','1','d','d','-','b','d','0','b','-','0','8','0','0','2','0','0','c','9','a','6','6','}',0x0000}  //bPropertyData - this is the actual GUID value.  Make sure this matches the PC application code trying to connect to the device.
    };    
#endif

#if defined(__18CXX)
    #pragma code
#endif

#endif
/** EOF usb_descriptors.c ****************************************************/
