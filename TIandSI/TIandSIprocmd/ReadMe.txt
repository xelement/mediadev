TISIprog
(TISI Professional Graphical)

������ Lei Xiaohua
leixiaohua1020@126.com
�й���ý��ѧ/���ֵ��Ӽ���
Communication University of China / Digital TV Technology

http://blog.csdn.net/leixiaohua1020

�����̿��Լ���һ��ѹ����Ƶ��ʱ����ϢTI��Temporal perceptual Information��
Ҳ���Գ�ʱ�临�Ӷȣ��Ϳռ���ϢSI��Spatial perceptual Information��Ҳ����
�ƿռ临�Ӷȣ������㷽�����Ա�׼��ITU-R BT.1788

Professional: ֧��ѹ���������������������ݱ���YUV��RGB����Ϊ����
Graphical: ֧��ͼ�λ���ʾTI��ʱ����Ϣ����SI���ռ���Ϣ��

This software can calculate a video bitstream's TI(Temporal perceptual Information) 
and SI(Spatial perceptual Information) according to ITU-R BT.1788.

Professional: Support bitstreams (not raw data such as YUV, RGB, etc.) as Input.
Graphical:Support graphically show TI and SI.


Help

----------------------------------------
Options:
-i      Input file URL
-o      Output.csv file URL [Default is {Input Name}.csv]
-l      Limit calculate frame number
-g      Show result Graphically [Option is 'ti' or 'si']
-n      Interval to show result to the screen [Default is 5]
-h      Show this text.
----------------------------------------
Examples:
TISIprog -i test.avi
TISIprog -i test.avi -o test_cal.csv
TISIprog -i test.avi -g ti