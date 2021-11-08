EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 5
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
L power:GND #PWR?
U 1 1 5DB4AADD
P 5100 1250
AR Path="/61B8DF6C/5DB4AADD" Ref="#PWR?"  Part="1" 
AR Path="/61BBA2C8/5DB4AADD" Ref="#PWR?"  Part="1" 
AR Path="/61BC6B02/5DB4AADD" Ref="#PWR?"  Part="1" 
AR Path="/61CBD8BC/5DB4AADD" Ref="#PWR?"  Part="1" 
AR Path="/61CCA493/5DB4AADD" Ref="#PWR?"  Part="1" 
AR Path="/61CE5E8C/5DB4AADD" Ref="#PWR?"  Part="1" 
AR Path="/61D24785/5DB4AADD" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5100 1000 50  0001 C CNN
F 1 "GND" H 5105 1077 50  0000 C CNN
F 2 "" H 5100 1250 50  0001 C CNN
F 3 "" H 5100 1250 50  0001 C CNN
	1    5100 1250
	1    0    0    -1  
$EndComp
Text HLabel 4700 1550 2    50   Input ~ 0
MOSI
Text HLabel 4700 1650 2    50   Output ~ 0
MISO
Wire Wire Line
	4250 1550 4700 1550
Wire Wire Line
	4250 1650 4700 1650
$Comp
L power:GND #PWR?
U 1 1 5DB760F5
P 2850 1300
AR Path="/61B8DF6C/5DB760F5" Ref="#PWR?"  Part="1" 
AR Path="/61BBA2C8/5DB760F5" Ref="#PWR?"  Part="1" 
AR Path="/61BC6B02/5DB760F5" Ref="#PWR?"  Part="1" 
AR Path="/61CBD8BC/5DB760F5" Ref="#PWR?"  Part="1" 
AR Path="/61CCA493/5DB760F5" Ref="#PWR?"  Part="1" 
AR Path="/61CE5E8C/5DB760F5" Ref="#PWR?"  Part="1" 
AR Path="/61D24785/5DB760F5" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2850 1050 50  0001 C CNN
F 1 "GND" H 2855 1127 50  0000 C CNN
F 2 "" H 2850 1300 50  0001 C CNN
F 3 "" H 2850 1300 50  0001 C CNN
	1    2850 1300
	1    0    0    -1  
$EndComp
Text HLabel 4700 1750 2    50   Input ~ 0
SCLK
Wire Wire Line
	4250 1750 4700 1750
Text HLabel 4700 1950 2    50   Input ~ 0
~CS
Wire Wire Line
	4700 1950 4250 1950
Text HLabel 4700 1850 2    50   Output ~ 0
~DRDY
Wire Wire Line
	4250 1850 4700 1850
$Comp
L Device:C C?
U 1 1 5DB818AE
P 4500 1350
AR Path="/61B8DF6C/5DB818AE" Ref="C?"  Part="1" 
AR Path="/61BBA2C8/5DB818AE" Ref="C?"  Part="1" 
AR Path="/61BC6B02/5DB818AE" Ref="C?"  Part="1" 
AR Path="/61CBD8BC/5DB818AE" Ref="C?"  Part="1" 
AR Path="/61CCA493/5DB818AE" Ref="C?"  Part="1" 
AR Path="/61CE5E8C/5DB818AE" Ref="C?"  Part="1" 
AR Path="/61D24785/5DB818AE" Ref="C?"  Part="1" 
F 0 "C?" V 4248 1350 50  0000 C CNN
F 1 "220nF" V 4339 1350 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4538 1200 50  0001 C CNN
F 3 "~" H 4500 1350 50  0001 C CNN
	1    4500 1350
	0    1    1    0   
$EndComp
Wire Wire Line
	4250 1350 4350 1350
Wire Wire Line
	4250 1250 4650 1250
Wire Wire Line
	4650 1250 4650 1350
$Comp
L power:+3V3 #PWR?
U 1 1 5DB84339
P 4300 1000
AR Path="/61B8DF6C/5DB84339" Ref="#PWR?"  Part="1" 
AR Path="/61BBA2C8/5DB84339" Ref="#PWR?"  Part="1" 
AR Path="/61BC6B02/5DB84339" Ref="#PWR?"  Part="1" 
AR Path="/61CBD8BC/5DB84339" Ref="#PWR?"  Part="1" 
AR Path="/61CCA493/5DB84339" Ref="#PWR?"  Part="1" 
AR Path="/61CE5E8C/5DB84339" Ref="#PWR?"  Part="1" 
AR Path="/61D24785/5DB84339" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4300 850 50  0001 C CNN
F 1 "+3V3" H 4315 1173 50  0000 C CNN
F 2 "" H 4300 1000 50  0001 C CNN
F 3 "" H 4300 1000 50  0001 C CNN
	1    4300 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 1000 4300 1150
Wire Wire Line
	4300 1150 4250 1150
$Comp
L power:+3V3 #PWR?
U 1 1 5DB85107
P 3200 1000
AR Path="/61B8DF6C/5DB85107" Ref="#PWR?"  Part="1" 
AR Path="/61BBA2C8/5DB85107" Ref="#PWR?"  Part="1" 
AR Path="/61BC6B02/5DB85107" Ref="#PWR?"  Part="1" 
AR Path="/61CBD8BC/5DB85107" Ref="#PWR?"  Part="1" 
AR Path="/61CCA493/5DB85107" Ref="#PWR?"  Part="1" 
AR Path="/61CE5E8C/5DB85107" Ref="#PWR?"  Part="1" 
AR Path="/61D24785/5DB85107" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3200 850 50  0001 C CNN
F 1 "+3V3" H 3215 1173 50  0000 C CNN
F 2 "" H 3200 1000 50  0001 C CNN
F 3 "" H 3200 1000 50  0001 C CNN
	1    3200 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 1150 3200 1150
Wire Wire Line
	3200 1150 3200 1000
Wire Wire Line
	2200 1200 2600 1200
Wire Wire Line
	2200 900  2600 900 
Text Label 2600 900  2    50   ~ 0
AIN0P
Wire Wire Line
	3250 1350 3000 1350
Text Label 3000 1350 0    50   ~ 0
AIN0P
Wire Wire Line
	3250 1450 3000 1450
Wire Wire Line
	3250 1550 3000 1550
Wire Wire Line
	3250 1650 3000 1650
Wire Wire Line
	3250 1750 3000 1750
Wire Wire Line
	3250 1850 3000 1850
Wire Wire Line
	3250 1950 3000 1950
Wire Wire Line
	3250 2050 3000 2050
Text Label 3000 1450 0    50   ~ 0
AIN0N
Text Label 3000 1550 0    50   ~ 0
AIN1N
Text Label 3000 1650 0    50   ~ 0
AIN1P
Text Label 3000 1750 0    50   ~ 0
AIN2P
Text Label 3000 1850 0    50   ~ 0
AIN2N
Text Label 3000 1950 0    50   ~ 0
AIN3N
Text Label 3000 2050 0    50   ~ 0
AIN3P
Text Label 2600 1200 2    50   ~ 0
AIN0N
Wire Wire Line
	2200 1750 2600 1750
Wire Wire Line
	2200 2050 2600 2050
Text Label 2600 2050 2    50   ~ 0
AIN1N
Text Label 2600 1750 2    50   ~ 0
AIN1P
Text Label 2600 2700 2    50   ~ 0
AIN2P
Text Label 2600 3000 2    50   ~ 0
AIN2N
$Comp
L Device:C C?
U 1 1 5DBA080B
P 2200 3750
AR Path="/61B8DF6C/5DBA080B" Ref="C?"  Part="1" 
AR Path="/61BBA2C8/5DBA080B" Ref="C?"  Part="1" 
AR Path="/61BC6B02/5DBA080B" Ref="C?"  Part="1" 
AR Path="/61CBD8BC/5DBA080B" Ref="C?"  Part="1" 
AR Path="/61CCA493/5DBA080B" Ref="C?"  Part="1" 
AR Path="/61CE5E8C/5DBA080B" Ref="C?"  Part="1" 
AR Path="/61D24785/5DBA080B" Ref="C?"  Part="1" 
F 0 "C?" H 2315 3796 50  0000 L CNN
F 1 "10nF" H 2315 3705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2238 3600 50  0001 C CNN
F 3 "~" H 2200 3750 50  0001 C CNN
	1    2200 3750
	1    0    0    1   
$EndComp
Wire Wire Line
	2200 3950 2200 3900
Wire Wire Line
	2200 3600 2600 3600
Wire Wire Line
	2200 3900 2600 3900
Text Label 2600 3900 2    50   ~ 0
AIN3N
Text Label 2600 3600 2    50   ~ 0
AIN3P
Connection ~ 4650 1250
Text HLabel 4700 1450 2    50   Input ~ 0
CLKIN
Wire Wire Line
	4250 1450 4700 1450
$Comp
L Device:Jumper_NO_Small JP?
U 1 1 5DB8E945
P 2200 4050
AR Path="/61B8DF6C/5DB8E945" Ref="JP?"  Part="1" 
AR Path="/61BBA2C8/5DB8E945" Ref="JP?"  Part="1" 
AR Path="/61BC6B02/5DB8E945" Ref="JP?"  Part="1" 
AR Path="/61CBD8BC/5DB8E945" Ref="JP?"  Part="1" 
AR Path="/61CCA493/5DB8E945" Ref="JP?"  Part="1" 
AR Path="/61CE5E8C/5DB8E945" Ref="JP?"  Part="1" 
AR Path="/61D24785/5DB8E945" Ref="JP?"  Part="1" 
F 0 "JP?" H 2200 4314 50  0000 C CNN
F 1 "Jumper" H 2200 4223 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2200 4050 50  0001 C CNN
F 3 "~" H 2200 4050 50  0001 C CNN
	1    2200 4050
	0    -1   1    0   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 5DBB407D
P 1550 3700
AR Path="/61B8DF6C/5DBB407D" Ref="J?"  Part="1" 
AR Path="/61BBA2C8/5DBB407D" Ref="J?"  Part="1" 
AR Path="/61BC6B02/5DBB407D" Ref="J?"  Part="1" 
AR Path="/61CBD8BC/5DBB407D" Ref="J?"  Part="1" 
AR Path="/61CCA493/5DBB407D" Ref="J?"  Part="1" 
AR Path="/61CE5E8C/5DBB407D" Ref="J?"  Part="1" 
AR Path="/61D24785/5DBB407D" Ref="J?"  Part="1" 
F 0 "J?" H 1468 3475 50  0000 C CNN
F 1 "Screw_Terminal_01x01" H 1468 3566 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-2.54mm_2pol" H 1550 3700 50  0001 C CNN
F 3 "~" H 1550 3700 50  0001 C CNN
	1    1550 3700
	-1   0    0    -1  
$EndComp
Connection ~ 2200 3600
Wire Wire Line
	2200 4150 2200 4200
$Comp
L power:GND #PWR?
U 1 1 5DB8E94B
P 2200 4200
AR Path="/61B8DF6C/5DB8E94B" Ref="#PWR?"  Part="1" 
AR Path="/61BBA2C8/5DB8E94B" Ref="#PWR?"  Part="1" 
AR Path="/61BC6B02/5DB8E94B" Ref="#PWR?"  Part="1" 
AR Path="/61CBD8BC/5DB8E94B" Ref="#PWR?"  Part="1" 
AR Path="/61CCA493/5DB8E94B" Ref="#PWR?"  Part="1" 
AR Path="/61CE5E8C/5DB8E94B" Ref="#PWR?"  Part="1" 
AR Path="/61D24785/5DB8E94B" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2200 3950 50  0001 C CNN
F 1 "GND" H 2205 4027 50  0000 C CNN
F 2 "" H 2200 4200 50  0001 C CNN
F 3 "" H 2200 4200 50  0001 C CNN
	1    2200 4200
	-1   0    0    -1  
$EndComp
Connection ~ 2200 3900
Wire Wire Line
	1750 3700 1950 3700
Wire Wire Line
	1950 3700 1950 3600
Wire Wire Line
	1950 3600 2200 3600
Wire Wire Line
	1750 3800 1950 3800
Wire Wire Line
	1950 3800 1950 3900
Wire Wire Line
	1950 3900 2200 3900
Wire Wire Line
	2200 3000 2600 3000
$Comp
L Device:C C?
U 1 1 5DC4321A
P 2200 2850
AR Path="/61B8DF6C/5DC4321A" Ref="C?"  Part="1" 
AR Path="/61BBA2C8/5DC4321A" Ref="C?"  Part="1" 
AR Path="/61BC6B02/5DC4321A" Ref="C?"  Part="1" 
AR Path="/61CBD8BC/5DC4321A" Ref="C?"  Part="1" 
AR Path="/61CCA493/5DC4321A" Ref="C?"  Part="1" 
AR Path="/61CE5E8C/5DC4321A" Ref="C?"  Part="1" 
AR Path="/61D24785/5DC4321A" Ref="C?"  Part="1" 
F 0 "C?" H 2315 2896 50  0000 L CNN
F 1 "10nF" H 2315 2805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2238 2700 50  0001 C CNN
F 3 "~" H 2200 2850 50  0001 C CNN
	1    2200 2850
	1    0    0    1   
$EndComp
Wire Wire Line
	2200 3050 2200 3000
$Comp
L Device:Jumper_NO_Small JP?
U 1 1 5DC43227
P 2200 3150
AR Path="/61B8DF6C/5DC43227" Ref="JP?"  Part="1" 
AR Path="/61BBA2C8/5DC43227" Ref="JP?"  Part="1" 
AR Path="/61BC6B02/5DC43227" Ref="JP?"  Part="1" 
AR Path="/61CBD8BC/5DC43227" Ref="JP?"  Part="1" 
AR Path="/61CCA493/5DC43227" Ref="JP?"  Part="1" 
AR Path="/61CE5E8C/5DC43227" Ref="JP?"  Part="1" 
AR Path="/61D24785/5DC43227" Ref="JP?"  Part="1" 
F 0 "JP?" H 2200 3414 50  0000 C CNN
F 1 "Jumper" H 2200 3323 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2200 3150 50  0001 C CNN
F 3 "~" H 2200 3150 50  0001 C CNN
	1    2200 3150
	0    -1   1    0   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 5DC43232
P 1550 2800
AR Path="/61B8DF6C/5DC43232" Ref="J?"  Part="1" 
AR Path="/61BBA2C8/5DC43232" Ref="J?"  Part="1" 
AR Path="/61BC6B02/5DC43232" Ref="J?"  Part="1" 
AR Path="/61CBD8BC/5DC43232" Ref="J?"  Part="1" 
AR Path="/61CCA493/5DC43232" Ref="J?"  Part="1" 
AR Path="/61CE5E8C/5DC43232" Ref="J?"  Part="1" 
AR Path="/61D24785/5DC43232" Ref="J?"  Part="1" 
F 0 "J?" H 1468 2575 50  0000 C CNN
F 1 "Screw_Terminal_01x01" H 1468 2666 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-2.54mm_2pol" H 1550 2800 50  0001 C CNN
F 3 "~" H 1550 2800 50  0001 C CNN
	1    1550 2800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2200 3250 2200 3300
$Comp
L power:GND #PWR?
U 1 1 5DC4323E
P 2200 3300
AR Path="/61B8DF6C/5DC4323E" Ref="#PWR?"  Part="1" 
AR Path="/61BBA2C8/5DC4323E" Ref="#PWR?"  Part="1" 
AR Path="/61BC6B02/5DC4323E" Ref="#PWR?"  Part="1" 
AR Path="/61CBD8BC/5DC4323E" Ref="#PWR?"  Part="1" 
AR Path="/61CCA493/5DC4323E" Ref="#PWR?"  Part="1" 
AR Path="/61CE5E8C/5DC4323E" Ref="#PWR?"  Part="1" 
AR Path="/61D24785/5DC4323E" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2200 3050 50  0001 C CNN
F 1 "GND" H 2205 3127 50  0000 C CNN
F 2 "" H 2200 3300 50  0001 C CNN
F 3 "" H 2200 3300 50  0001 C CNN
	1    2200 3300
	-1   0    0    -1  
$EndComp
Connection ~ 2200 3000
Wire Wire Line
	1750 2800 1950 2800
Wire Wire Line
	1950 2800 1950 2700
Wire Wire Line
	1950 2700 2200 2700
Wire Wire Line
	1750 2900 1950 2900
Wire Wire Line
	1950 2900 1950 3000
Wire Wire Line
	1950 3000 2200 3000
Connection ~ 2200 2700
Wire Wire Line
	2200 2700 2600 2700
$Comp
L Device:C C?
U 1 1 5DC47EB7
P 2200 1900
AR Path="/61B8DF6C/5DC47EB7" Ref="C?"  Part="1" 
AR Path="/61BBA2C8/5DC47EB7" Ref="C?"  Part="1" 
AR Path="/61BC6B02/5DC47EB7" Ref="C?"  Part="1" 
AR Path="/61CBD8BC/5DC47EB7" Ref="C?"  Part="1" 
AR Path="/61CCA493/5DC47EB7" Ref="C?"  Part="1" 
AR Path="/61CE5E8C/5DC47EB7" Ref="C?"  Part="1" 
AR Path="/61D24785/5DC47EB7" Ref="C?"  Part="1" 
F 0 "C?" H 2315 1946 50  0000 L CNN
F 1 "10nF" H 2315 1855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2238 1750 50  0001 C CNN
F 3 "~" H 2200 1900 50  0001 C CNN
	1    2200 1900
	1    0    0    1   
$EndComp
Wire Wire Line
	2200 2100 2200 2050
$Comp
L Device:Jumper_NO_Small JP?
U 1 1 5DC47EC2
P 2200 2200
AR Path="/61B8DF6C/5DC47EC2" Ref="JP?"  Part="1" 
AR Path="/61BBA2C8/5DC47EC2" Ref="JP?"  Part="1" 
AR Path="/61BC6B02/5DC47EC2" Ref="JP?"  Part="1" 
AR Path="/61CBD8BC/5DC47EC2" Ref="JP?"  Part="1" 
AR Path="/61CCA493/5DC47EC2" Ref="JP?"  Part="1" 
AR Path="/61CE5E8C/5DC47EC2" Ref="JP?"  Part="1" 
AR Path="/61D24785/5DC47EC2" Ref="JP?"  Part="1" 
F 0 "JP?" H 2200 2464 50  0000 C CNN
F 1 "Jumper" H 2200 2373 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2200 2200 50  0001 C CNN
F 3 "~" H 2200 2200 50  0001 C CNN
	1    2200 2200
	0    -1   1    0   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 5DC47ECD
P 1550 1850
AR Path="/61B8DF6C/5DC47ECD" Ref="J?"  Part="1" 
AR Path="/61BBA2C8/5DC47ECD" Ref="J?"  Part="1" 
AR Path="/61BC6B02/5DC47ECD" Ref="J?"  Part="1" 
AR Path="/61CBD8BC/5DC47ECD" Ref="J?"  Part="1" 
AR Path="/61CCA493/5DC47ECD" Ref="J?"  Part="1" 
AR Path="/61CE5E8C/5DC47ECD" Ref="J?"  Part="1" 
AR Path="/61D24785/5DC47ECD" Ref="J?"  Part="1" 
F 0 "J?" H 1468 1625 50  0000 C CNN
F 1 "Screw_Terminal_01x01" H 1468 1716 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-2.54mm_2pol" H 1550 1850 50  0001 C CNN
F 3 "~" H 1550 1850 50  0001 C CNN
	1    1550 1850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2200 2300 2200 2350
$Comp
L power:GND #PWR?
U 1 1 5DC47ED8
P 2200 2350
AR Path="/61B8DF6C/5DC47ED8" Ref="#PWR?"  Part="1" 
AR Path="/61BBA2C8/5DC47ED8" Ref="#PWR?"  Part="1" 
AR Path="/61BC6B02/5DC47ED8" Ref="#PWR?"  Part="1" 
AR Path="/61CBD8BC/5DC47ED8" Ref="#PWR?"  Part="1" 
AR Path="/61CCA493/5DC47ED8" Ref="#PWR?"  Part="1" 
AR Path="/61CE5E8C/5DC47ED8" Ref="#PWR?"  Part="1" 
AR Path="/61D24785/5DC47ED8" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2200 2100 50  0001 C CNN
F 1 "GND" H 2205 2177 50  0000 C CNN
F 2 "" H 2200 2350 50  0001 C CNN
F 3 "" H 2200 2350 50  0001 C CNN
	1    2200 2350
	-1   0    0    -1  
$EndComp
Connection ~ 2200 2050
Wire Wire Line
	1750 1850 1950 1850
Wire Wire Line
	1950 1850 1950 1750
Wire Wire Line
	1950 1750 2200 1750
Wire Wire Line
	1750 1950 1950 1950
Wire Wire Line
	1950 1950 1950 2050
Wire Wire Line
	1950 2050 2200 2050
Connection ~ 2200 1750
$Comp
L Device:C C?
U 1 1 5DC4E705
P 2200 1050
AR Path="/61B8DF6C/5DC4E705" Ref="C?"  Part="1" 
AR Path="/61BBA2C8/5DC4E705" Ref="C?"  Part="1" 
AR Path="/61BC6B02/5DC4E705" Ref="C?"  Part="1" 
AR Path="/61CBD8BC/5DC4E705" Ref="C?"  Part="1" 
AR Path="/61CCA493/5DC4E705" Ref="C?"  Part="1" 
AR Path="/61CE5E8C/5DC4E705" Ref="C?"  Part="1" 
AR Path="/61D24785/5DC4E705" Ref="C?"  Part="1" 
F 0 "C?" H 2315 1096 50  0000 L CNN
F 1 "10nF" H 2315 1005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2238 900 50  0001 C CNN
F 3 "~" H 2200 1050 50  0001 C CNN
	1    2200 1050
	1    0    0    1   
$EndComp
Wire Wire Line
	2200 1250 2200 1200
$Comp
L Device:Jumper_NO_Small JP?
U 1 1 5DC4E710
P 2200 1350
AR Path="/61B8DF6C/5DC4E710" Ref="JP?"  Part="1" 
AR Path="/61BBA2C8/5DC4E710" Ref="JP?"  Part="1" 
AR Path="/61BC6B02/5DC4E710" Ref="JP?"  Part="1" 
AR Path="/61CBD8BC/5DC4E710" Ref="JP?"  Part="1" 
AR Path="/61CCA493/5DC4E710" Ref="JP?"  Part="1" 
AR Path="/61CE5E8C/5DC4E710" Ref="JP?"  Part="1" 
AR Path="/61D24785/5DC4E710" Ref="JP?"  Part="1" 
F 0 "JP?" H 2200 1614 50  0000 C CNN
F 1 "Jumper" H 2200 1523 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2200 1350 50  0001 C CNN
F 3 "~" H 2200 1350 50  0001 C CNN
	1    2200 1350
	0    -1   1    0   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 5DC4E71A
P 1550 1000
AR Path="/61B8DF6C/5DC4E71A" Ref="J?"  Part="1" 
AR Path="/61BBA2C8/5DC4E71A" Ref="J?"  Part="1" 
AR Path="/61BC6B02/5DC4E71A" Ref="J?"  Part="1" 
AR Path="/61CBD8BC/5DC4E71A" Ref="J?"  Part="1" 
AR Path="/61CCA493/5DC4E71A" Ref="J?"  Part="1" 
AR Path="/61CE5E8C/5DC4E71A" Ref="J?"  Part="1" 
AR Path="/61D24785/5DC4E71A" Ref="J?"  Part="1" 
F 0 "J?" H 1468 775 50  0000 C CNN
F 1 "Screw_Terminal_01x01" H 1468 866 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-2.54mm_2pol" H 1550 1000 50  0001 C CNN
F 3 "~" H 1550 1000 50  0001 C CNN
	1    1550 1000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2200 1450 2200 1500
$Comp
L power:GND #PWR?
U 1 1 5DC4E725
P 2200 1500
AR Path="/61B8DF6C/5DC4E725" Ref="#PWR?"  Part="1" 
AR Path="/61BBA2C8/5DC4E725" Ref="#PWR?"  Part="1" 
AR Path="/61BC6B02/5DC4E725" Ref="#PWR?"  Part="1" 
AR Path="/61CBD8BC/5DC4E725" Ref="#PWR?"  Part="1" 
AR Path="/61CCA493/5DC4E725" Ref="#PWR?"  Part="1" 
AR Path="/61CE5E8C/5DC4E725" Ref="#PWR?"  Part="1" 
AR Path="/61D24785/5DC4E725" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2200 1250 50  0001 C CNN
F 1 "GND" H 2205 1327 50  0000 C CNN
F 2 "" H 2200 1500 50  0001 C CNN
F 3 "" H 2200 1500 50  0001 C CNN
	1    2200 1500
	-1   0    0    -1  
$EndComp
Connection ~ 2200 1200
Wire Wire Line
	1750 1000 1950 1000
Wire Wire Line
	1950 1000 1950 900 
Wire Wire Line
	1950 900  2200 900 
Wire Wire Line
	1750 1100 1950 1100
Wire Wire Line
	1950 1100 1950 1200
Wire Wire Line
	1950 1200 2200 1200
Connection ~ 2200 900 
Wire Wire Line
	2850 1250 3250 1250
Wire Wire Line
	2850 1300 2850 1250
Wire Wire Line
	5300 1250 5300 1450
Wire Wire Line
	4650 1250 5100 1250
$Comp
L Device:C C?
U 1 1 5E1BAB7F
P 5850 1600
AR Path="/61B8DF6C/5E1BAB7F" Ref="C?"  Part="1" 
AR Path="/61BBA2C8/5E1BAB7F" Ref="C?"  Part="1" 
AR Path="/61BC6B02/5E1BAB7F" Ref="C?"  Part="1" 
AR Path="/61CBD8BC/5E1BAB7F" Ref="C?"  Part="1" 
AR Path="/61CCA493/5E1BAB7F" Ref="C?"  Part="1" 
AR Path="/61CE5E8C/5E1BAB7F" Ref="C?"  Part="1" 
AR Path="/61D24785/5E1BAB7F" Ref="C?"  Part="1" 
F 0 "C?" H 5965 1646 50  0000 L CNN
F 1 "100nF" H 5965 1555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5888 1450 50  0001 C CNN
F 3 "~" H 5850 1600 50  0001 C CNN
	1    5850 1600
	1    0    0    1   
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5E1BB1FA
P 5850 1400
AR Path="/61B8DF6C/5E1BB1FA" Ref="#PWR?"  Part="1" 
AR Path="/61BBA2C8/5E1BB1FA" Ref="#PWR?"  Part="1" 
AR Path="/61BC6B02/5E1BB1FA" Ref="#PWR?"  Part="1" 
AR Path="/61CBD8BC/5E1BB1FA" Ref="#PWR?"  Part="1" 
AR Path="/61CCA493/5E1BB1FA" Ref="#PWR?"  Part="1" 
AR Path="/61CE5E8C/5E1BB1FA" Ref="#PWR?"  Part="1" 
AR Path="/61D24785/5E1BB1FA" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5850 1250 50  0001 C CNN
F 1 "+3V3" H 5865 1573 50  0000 C CNN
F 2 "" H 5850 1400 50  0001 C CNN
F 3 "" H 5850 1400 50  0001 C CNN
	1    5850 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E1BB739
P 5850 1800
AR Path="/61B8DF6C/5E1BB739" Ref="#PWR?"  Part="1" 
AR Path="/61BBA2C8/5E1BB739" Ref="#PWR?"  Part="1" 
AR Path="/61BC6B02/5E1BB739" Ref="#PWR?"  Part="1" 
AR Path="/61CBD8BC/5E1BB739" Ref="#PWR?"  Part="1" 
AR Path="/61CCA493/5E1BB739" Ref="#PWR?"  Part="1" 
AR Path="/61CE5E8C/5E1BB739" Ref="#PWR?"  Part="1" 
AR Path="/61D24785/5E1BB739" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5850 1550 50  0001 C CNN
F 1 "GND" H 5855 1627 50  0000 C CNN
F 2 "" H 5850 1800 50  0001 C CNN
F 3 "" H 5850 1800 50  0001 C CNN
	1    5850 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 1800 5850 1750
Wire Wire Line
	5850 1450 5850 1400
$Comp
L iclr:ADS131M04 U?
U 1 1 61B95E91
P 3750 1600
AR Path="/61B8DF6C/61B95E91" Ref="U?"  Part="1" 
AR Path="/61BBA2C8/61B95E91" Ref="U?"  Part="1" 
AR Path="/61BC6B02/61B95E91" Ref="U?"  Part="1" 
AR Path="/61CBD8BC/61B95E91" Ref="U?"  Part="1" 
AR Path="/61CCA493/61B95E91" Ref="U?"  Part="1" 
AR Path="/61CE5E8C/61B95E91" Ref="U?"  Part="1" 
AR Path="/61D24785/61B95E91" Ref="U?"  Part="1" 
F 0 "U?" H 3750 2275 50  0000 C CNN
F 1 "ADS131M04" H 3750 2184 50  0000 C CNN
F 2 "" H 3550 1650 50  0001 C CNN
F 3 "" H 3550 1650 50  0001 C CNN
	1    3750 1600
	1    0    0    -1  
$EndComp
Text HLabel 4700 2050 2    50   Input ~ 0
SYNC
Wire Wire Line
	4700 2050 4250 2050
$EndSCHEMATC
