EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:Battery_Cell BT1
U 1 1 603DD1DE
P 5350 5250
F 0 "BT1" H 5468 5346 50  0000 L CNN
F 1 "Battery_Cell" H 5468 5255 50  0000 L CNN
F 2 "Battery_Holders:Keystone_2466_1xAAA" V 5350 5310 50  0001 C CNN
F 3 "~" V 5350 5310 50  0001 C CNN
	1    5350 5250
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPDT SW18
U 1 1 603DC391
P 6350 5850
F 0 "SW18" H 6350 6135 50  0000 C CNN
F 1 "SW_SPDT" H 6350 6044 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPDT_CK-JS102011SAQN" H 6350 5850 50  0001 C CNN
F 3 "~" H 6350 5850 50  0001 C CNN
	1    6350 5850
	1    0    0    -1  
$EndComp
NoConn ~ 10100 -50 
$Comp
L Device:Buzzer BZ?
U 1 1 606DB1EA
P 6400 3200
F 0 "BZ?" H 6552 3229 50  0000 L CNN
F 1 "Buzzer" H 6552 3138 50  0000 L CNN
F 2 "" V 6375 3300 50  0001 C CNN
F 3 "~" V 6375 3300 50  0001 C CNN
	1    6400 3200
	1    0    0    -1  
$EndComp
$Comp
L MCU_Microchip_ATtiny:ATtiny1614-SS U?
U 1 1 606DB82F
P 7100 1750
F 0 "U?" H 7100 2631 50  0000 C CNN
F 1 "ATtiny1614-SS" H 7100 2540 50  0000 C CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 7100 1750 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATtiny1614-data-sheet-40001995A.pdf" H 7100 1750 50  0001 C CNN
	1    7100 1750
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:MCP6001R U?
U 1 1 606DCE81
P 2300 2200
F 0 "U?" H 2644 2246 50  0000 L CNN
F 1 "MCP6001R" H 2644 2155 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 2300 2200 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 2300 2400 50  0001 C CNN
	1    2300 2200
	1    0    0    -1  
$EndComp
$Comp
L keyboard-13:keys U?
U 1 1 606DE30F
P 1950 5300
F 0 "U?" H 1950 5915 50  0000 C CNN
F 1 "keys" H 1950 5824 50  0000 C CNN
F 2 "" H 1800 5450 50  0001 C CNN
F 3 "" H 1800 5450 50  0001 C CNN
	1    1950 5300
	1    0    0    -1  
$EndComp
$Comp
L keyboard-13:keys U?
U 1 1 606DF264
P 3200 5300
F 0 "U?" H 3200 5915 50  0000 C CNN
F 1 "keys" H 3200 5824 50  0000 C CNN
F 2 "" H 3050 5450 50  0001 C CNN
F 3 "" H 3050 5450 50  0001 C CNN
	1    3200 5300
	1    0    0    -1  
$EndComp
$Comp
L QT60160:QT60160 U?
U 1 1 606E3789
P 8650 5000
F 0 "U?" H 9394 5046 50  0000 L CNN
F 1 "QT60160" H 9394 4955 50  0000 L CNN
F 2 "" H 8650 6250 50  0001 C CNN
F 3 "" H 8650 6250 50  0001 C CNN
	1    8650 5000
	1    0    0    -1  
$EndComp
$Comp
L MCU_Microchip_ATtiny:ATtiny3217-M U?
U 1 1 606E6858
P 10100 2150
F 0 "U?" H 10100 3431 50  0000 C CNN
F 1 "ATtiny3217-M" H 10100 3340 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-24-1EP_4x4mm_P0.5mm_EP2.6x2.6mm" H 10100 2150 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATtiny3217_1617-Data-Sheet-40001999B.pdf" H 10100 2150 50  0001 C CNN
	1    10100 2150
	1    0    0    -1  
$EndComp
$Comp
L MCU_Microchip_ATtiny:ATtiny3216-M U?
U 1 1 606E7532
P 8550 1950
F 0 "U?" H 8550 3031 50  0000 C CNN
F 1 "ATtiny3216-M" H 8550 2940 50  0000 C CNN
F 2 "Package_DFN_QFN:VQFN-20-1EP_3x3mm_P0.4mm_EP1.7x1.7mm" H 8550 1950 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATtiny3216_ATtiny1616-data-sheet-40001997B.pdf" H 8550 1950 50  0001 C CNN
	1    8550 1950
	1    0    0    -1  
$EndComp
NoConn ~ 9500 2250
NoConn ~ 9500 2350
NoConn ~ 9500 2450
NoConn ~ 9500 2550
NoConn ~ 9500 2650
NoConn ~ 9500 2750
NoConn ~ 10700 2650
NoConn ~ 10700 2750
NoConn ~ 10700 2850
NoConn ~ 10700 2950
$EndSCHEMATC
