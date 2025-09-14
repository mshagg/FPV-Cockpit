本仓库提供已经编译好的固件 factory.bin，可以直接刷写到 ESP32-S3 开发板，无需 Arduino IDE 编译。

1. 准备工作

一块 ESP32-S3 开发板（Lilygo T-display s3 amoled --V1.0 不带触摸 H619， FLASH：16MB；）。

PSRAM=opi；FlashMode=qio；

一根数据 USB 线。

安装 Python3 和 esptool.py：

python3 -m pip install --upgrade esptool

2. 下载固件

从仓库或提供的链接下载 factory.bin 到本地。

3. 查找串口

插上开发板后，在 macOS / Linux 下执行：

ls /dev/cu.*


找到类似 /dev/cu.usbmodemXXXX 或 /dev/cu.usbserial-XXXX 的设备名。

Windows 下在设备管理器中查看 COM 口号，例如 COM3。

4. 进入下载模式

大多数 ESP32-S3 板子无需手动操作，直接执行烧录命令即可。

如果出现 Connecting... 长时间无响应：

按住板子的 BOOT 键，执行烧录命令，看见“Connecting…”后松开；或轻按 EN/RESET 键。

5. 烧录命令

在命令行进入保存 factory.bin 的目录，执行：

macOS / Linux：

esptool.py --chip esp32s3 --port /dev/cu.usbmodemXXXX --baud 921600 \
  --before default_reset --after hard_reset \
  write_flash -z 0x0 factory.bin


Windows：

esptool.py --chip esp32s3 --port COM3 --baud 921600 ^
  --before default_reset --after hard_reset ^
  write_flash -z 0x0 factory.bin


把 --port 后面的串口名换成你自己的。

6. 烧录完成与运行

烧录成功后终端会显示 Hash of data verified. 和 Hard resetting via RTS pin…。
重新插拔 USB 或按一下 EN/RESET，开发板就会启动运行这个固件。

7. 打开串口监视器（可选）

如果需要查看输出日志，使用串口工具（Arduino IDE 串口监视器、minicom、Putty 等）打开对应串口，波特率设为 115200 或你程序中设定的值。