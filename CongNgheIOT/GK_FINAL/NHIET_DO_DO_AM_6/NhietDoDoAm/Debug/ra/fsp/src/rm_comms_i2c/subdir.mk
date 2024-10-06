################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/rm_comms_i2c/rm_comms_i2c.c \
../ra/fsp/src/rm_comms_i2c/rm_comms_i2c_driver_ra.c 

C_DEPS += \
./ra/fsp/src/rm_comms_i2c/rm_comms_i2c.d \
./ra/fsp/src/rm_comms_i2c/rm_comms_i2c_driver_ra.d 

OBJS += \
./ra/fsp/src/rm_comms_i2c/rm_comms_i2c.o \
./ra/fsp/src/rm_comms_i2c/rm_comms_i2c_driver_ra.o 

SREC += \
NhietDoDoAm.srec 

MAP += \
NhietDoDoAm.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/rm_comms_i2c/%.o: ../ra/fsp/src/rm_comms_i2c/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM33 -D_RA_ORDINAL=1 -I"D:/KySuBachKhoa/CongNgheIOT/NHIET_DO_DO_AM/NhietDoDoAm/src" -I"." -I"D:/KySuBachKhoa/CongNgheIOT/NHIET_DO_DO_AM/NhietDoDoAm/ra/fsp/inc" -I"D:/KySuBachKhoa/CongNgheIOT/NHIET_DO_DO_AM/NhietDoDoAm/ra/fsp/inc/api" -I"D:/KySuBachKhoa/CongNgheIOT/NHIET_DO_DO_AM/NhietDoDoAm/ra/fsp/inc/instances" -I"D:/KySuBachKhoa/CongNgheIOT/NHIET_DO_DO_AM/NhietDoDoAm/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"D:/KySuBachKhoa/CongNgheIOT/NHIET_DO_DO_AM/NhietDoDoAm/ra_gen" -I"D:/KySuBachKhoa/CongNgheIOT/NHIET_DO_DO_AM/NhietDoDoAm/ra_cfg/fsp_cfg/bsp" -I"D:/KySuBachKhoa/CongNgheIOT/NHIET_DO_DO_AM/NhietDoDoAm/ra_cfg/fsp_cfg" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

