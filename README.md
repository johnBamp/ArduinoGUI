# ArduinoGUI for ILI9341 TFT with Capacitive Touchscreen
## ArduinoGUI.h

This header file includes the necessary header files for the Arduino GUI. It includes Widget.h, Menu.h, Button.h, and ImageDrawer.h.

## Button

### Button.cpp

This file contains the implementation of the Button class and the doubleTapButton class. The Button class inherits from the Widget class and has a method to change the state of the button and a method to check if the button has been touched. The doubleTapButton class inherits from the Button class and overrides the checkTouch method to handle double taps.

### Button.h

This header file declares the Button class and the doubleTapButton class. The Button class inherits from the Widget class and has a method to change the state of the button and a method to check if the button has been touched. The doubleTapButton class inherits from the Button class and overrides the checkTouch method to handle double taps.

## Image Drawer

### ImageDrawer.cpp

This file contains the implementation of the ImageDrawer class. The ImageDrawer class has a method to draw an image from a file and a method to get the dimensions of an image.

### ImageDrawer.h

This header file declares the ImageDrawer class. The ImageDrawer class has a method to draw an image from a file and a method to get the dimensions of an image.

## Menu

### Menu.cpp

This file contains the implementation of the Menu class. The Menu class inherits from the Widget class and has methods to draw the menu, set the position and size of the menu, set the number of columns and rows in the menu, and add widgets to the menu.

### Menu.h

This header file declares the Menu class. The Menu class inherits from the Widget class and has methods to draw the menu, set the position and size of the menu, set the number of columns and rows in the menu, and add widgets to the menu.

## Style

### Style.h

This header file declares the Style struct. The Style struct contains various properties to style a widget, such as the background color, border color, text color, font size, and padding.

##Widget

###Widget.cpp

This file contains the implementation of the Widget class. The Widget class has methods to draw the widget, set the position and size of the widget, set the style of the widget, and set the text of the widget.

###Widget.h

This header file declares the Widget class. The Widget class has methods to draw the widget, set the position and size of the widget, set the style of the widget, and set the text of the widget.
