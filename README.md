# Introduce

As we know, Nvidia changed their power connector in 40 Series, which acting really bad.

So we create this monitor to make sure you can monitor the powerline(connector) load.

> So FUCK YOU NVIDIA. -- Linus Torvalds

# Hardware Design

Designed by <a href="https://github.com/SH-AXI" target="_blank"><text>SH_AXI<text/></a>, it's based on current sensor **INA3221**, and a powerful but cheap MCU **esp32-c3**. 

With his great design, the pcb can easily handle a huge current with a very low resistance.

You can check all you need to obtain in [**HERE**](fill_link_here).

# Software Design

<a href="https://github.com/n3wbeee" target="_blank"><text>n3wbeee<text/></a> did all the job in software, but he have to admit that he just coding shits and copy from everywhere.

Thanks **Arduino** and **u8g2** made things easier.

With the code he copied, this Monitor has smooth anmations when changing pages, like this:

![There should be a gif]()

Not bad, right?

# User Guidance

### Complie & Upload

This project based on Vscode plugin **PlatformIO**, go check it.

### Menu

I'll fill this param later, probably.