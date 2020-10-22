#ifndef CONFIG_H
#define CONFIG_H

// iMX6
#define IMX6_ADC0 "/sys/bus/iio/devices/iio:device0/in_voltage4_raw"
#define IMX6_ADC1 "/sys/bus/iio/devices/iio:device0/in_voltage5_raw"
#define IMX6_ADC2 "/sys/bus/iio/devices/iio:device0/in_voltage6_raw"
#define IMX6_ADC3 "/sys/bus/iio/devices/iio:device0/in_voltage7_raw"

// iM6ULL
#define IMX6ULL_ADC0 "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define IMX6ULL_ADC1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define IMX6ULL_ADC2 "/sys/bus/iio/devices/iio:device0/in_voltage8_raw"
#define IMX6ULL_ADC3 "/sys/bus/iio/devices/iio:device0/in_voltage9_raw"

// iMX7
#define IMX7_ADC0 "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define IMX7_ADC1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define IMX7_ADC2 "/sys/bus/iio/devices/iio:device0/in_voltage2_raw"
#define IMX7_ADC3 "/sys/bus/iio/devices/iio:device0/in_voltage3_raw"

#define SODIMM_45  1    // Input
#define SODIMM_102  145  // three
#define SODIMM_105  42
#define SODIMM_133  58
#define SODIMM_99  203 // four
#define SODIMM_104  59 // output
#define SODIMM_98  139 // output
#define SODIMM_135  2 // output


#define pwm1 "/sys/class/pwm/pwmchip0/"
#define pwm2 "/sys/class/pwm/pwmchip1/"
#define pwm3 "/sys/class/pwm/pwmchip2/"
#define pwm4 "/sys/class/pwm/pwmchip3/"
// iMX8X
#define IMX8X_ADC0 "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define IMX8X_ADC1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define IMX8X_ADC2 "/sys/bus/iio/devices/iio:device0/in_voltage4_raw"
#define IMX8X_ADC3 "/sys/bus/iio/devices/iio:device0/in_voltage5_raw"
#endif // CONFIG_H
