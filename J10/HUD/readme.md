本项目提供已经编译好的固件 factory.bin，可直接刷写到 ESP32-C3 开发板，无需 Arduino IDE 编译。

1. 硬件要求

ESP32-C3 开发板（ESP32-C3-SuperMini、4 MB Flash ，芯片型号ESP32C3FN4）。

一根数据 USB 线。

2. 安装烧录工具

在电脑上安装 Python 3，并安装 esptool：

python3 -m pip install --upgrade esptool

3. 下载固件

下载本仓库中的 factory.bin 到你的电脑。

4. 查找串口

插上开发板后，在 macOS / Linux 下执行：

ls /dev/cu.*


找到类似 /dev/cu.usbmodemXXXX 或 /dev/cu.usbserial-XXXX 的设备名。

Windows 在设备管理器中查看 COM 口号，例如 COM3。

5. 进入下载模式（如需要）

大多数 ESP32-C3 板子可直接烧录；如果出现 Connecting... 长时间无响应：

按住板子上的 BOOT 键，执行烧录命令，看见“Connecting…”后松开；或轻按 EN/RESET 键。

6. 烧录命令

在命令行进入保存 factory.bin 的目录，执行：

macOS / Linux：

esptool.py --chip esp32c3 --port /dev/cu.usbmodemXXXX --baud 921600 \
  --before default_reset --after hard_reset \
  write_flash -z 0x0 factory.bin


Windows：

esptool.py --chip esp32c3 --port COM3 --baud 921600 ^
  --before default_reset --after hard_reset ^
  write_flash -z 0x0 factory.bin


将 --port 后面的串口名替换为你自己的。

7. 烧录完成与运行

烧录成功后终端会显示 Hash of data verified. 和 Hard resetting via RTS pin…。

重新插拔 USB 或按一下 EN/RESET，开发板就会启动运行这个固件。

8. 串口监视器（可选）

如需查看运行日志，可使用 Arduino IDE 串口监视器、minicom 或 PuTTY 打开对应串口，波特率设为 115200 或程序中设定的值。