# makefile template project

这是针对arm cortex-M 系列MCU的模板工程。

## 使用的工具

- [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm)
- [pyocd](https://github.com/pyocd/pyOCD)
- make
- vscode，推荐插件：
    - Cortex-Debug
    - C/C++
    - Doxygen Documentation Generato

## 目前支持

- AIR:
    - AIR001 使用PY32F002A替换
    - AIR32F103XB（由于涨价，暂时不更新）
- APM:
    - APM32F103：
        - 基础例程
        - CPP
        - RT-Thread-nano

## 最新的makefile请使用APM32的例程中的基础历程或者RTT例程