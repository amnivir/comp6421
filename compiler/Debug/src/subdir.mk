################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/LexicalAnalyzer.cc \
../src/Syntatic.cc 

CC_DEPS += \
./src/LexicalAnalyzer.d \
./src/Syntatic.d 

OBJS += \
./src/LexicalAnalyzer.o \
./src/Syntatic.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -D__GXX_EXPERIMENTAL_CXX0X__ -I"/home/eshinig/comp/6421/compiler/externalLib" -I"/home/eshinig/comp/6421/compiler/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


