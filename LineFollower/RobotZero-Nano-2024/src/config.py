import sys
import re
from PyQt6.QtWidgets import (QApplication, QMainWindow, QWidget, QTabWidget,
                            QVBoxLayout, QHBoxLayout, QFormLayout, QLabel,
                            QSpinBox, QDoubleSpinBox, QPushButton, QFileDialog,
                            QMessageBox, QGroupBox)
from PyQt6.QtCore import Qt
from typing import Dict, Union, Any, Pattern
import copy

class ConfigParser:
    @staticmethod
    def extract_value(line: str, pattern: Pattern[str]) -> Union[int, float, None]:
        match = pattern.search(line)
        if match:
            value_str = match.group(1).strip('f')  # Remove 'f' suffix for floats
            try:
                return float(value_str) if '.' in value_str else int(value_str)
            except ValueError:
                return None
        return None

    @staticmethod
    def parse_config_file(content: str) -> Dict[str, Any]:
        params = {}
        # Pattern matches both integer and floating-point values
        pattern = re.compile(r'static\s+constexpr\s+(?:uint8_t|uint16_t|float)\s+(\w+)\s*=\s*([0-9.-]+f?)\s*;')
        
        for line in content.split('\n'):
            match = pattern.search(line)
            if match:
                param_name = match.group(1)
                value_str = match.group(2).strip('f')
                try:
                    params[param_name] = float(value_str) if '.' in value_str else int(value_str)
                except ValueError:
                    continue
        return params

class ConfigTab(QWidget):
    def __init__(self, parameters: Dict[str, dict]):
        super().__init__()
        self.parameters = copy.deepcopy(parameters)
        self.widgets = {}
        self.setup_ui()

    def setup_ui(self):
        layout = QVBoxLayout(self)
        
        for group_name, params in self.parameters.items():
            group = QGroupBox(group_name)
            group_layout = QFormLayout()
            
            for param_name, param_info in params.items():
                if isinstance(param_info['default'], float):
                    widget = QDoubleSpinBox()
                    widget.setDecimals(3)
                    widget.setRange(param_info['min'], param_info['max'])
                    widget.setValue(param_info['default'])
                else:
                    widget = QSpinBox()
                    widget.setRange(param_info['min'], param_info['max'])
                    widget.setValue(param_info['default'])
                
                self.widgets[param_name] = widget
                group_layout.addRow(param_name, widget)
            
            group.setLayout(group_layout)
            layout.addWidget(group)

    def update_values(self, params: Dict[str, Any]):
        for param_name, value in params.items():
            if param_name in self.widgets:
                self.widgets[param_name].setValue(value)

    def get_values(self) -> Dict[str, Any]:
        return {name: widget.value() for name, widget in self.widgets.items()}

class ConfigEditor(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Robot Configuration Editor")
        self.setMinimumSize(800, 600)
        
        # Define parameter groups with their ranges and default values
        self.parameters = {
            "Speed Parameters": {
                "SPEED_STARTUP": {"default": 140, "min": 0, "max": 255},
                "SPEED_TURN": {"default": 160, "min": 0, "max": 255},
                "SPEED_BRAKE": {"default": 180, "min": 0, "max": 255},
                "SPEED_CRUISE": {"default": 200, "min": 0, "max": 255},
                "SPEED_SLOW": {"default": 220, "min": 0, "max": 255},
                "SPEED_FAST": {"default": 235, "min": 0, "max": 255},
                "BASE_SLOW": {"default": 160, "min": 0, "max": 255},
                "BASE_FAST": {"default": 200, "min": 0, "max": 255},
            },
            "Control Parameters": {
                "K_PROPORTIONAL_DEFAULT": {"default": 7.5, "min": 0, "max": 20.0},
                "K_DERIVATIVE_DEFAULT": {"default": 900.0, "min": 0, "max": 2000.0},
                "FILTER_COEFFICIENT_DEFAULT": {"default": 0.8, "min": 0, "max": 1.0},
                "TURN_THRESHOLD": {"default": 35, "min": 0, "max": 100},
                "STRAIGHT_THRESHOLD": {"default": 12, "min": 0, "max": 50},
            },
            "Sensor Parameters": {
                "SENSOR_WEIGHT_S1": {"default": -3.0, "min": -10.0, "max": 10.0},
                "SENSOR_WEIGHT_S2": {"default": -1.5, "min": -10.0, "max": 10.0},
                "SENSOR_WEIGHT_S3": {"default": -0.8, "min": -10.0, "max": 10.0},
                "SENSOR_WEIGHT_S4": {"default": 0.8, "min": -10.0, "max": 10.0},
                "SENSOR_WEIGHT_S5": {"default": 1.5, "min": -10.0, "max": 10.0},
                "SENSOR_WEIGHT_S6": {"default": 3.0, "min": -10.0, "max": 10.0},
            },
            "Timing Parameters": {
                "SETUP_DELAY": {"default": 400, "min": 0, "max": 2000},
                "CALIBRATION_DELAY": {"default": 20, "min": 0, "max": 100},
                "MARKER_READ_INTERVAL": {"default": 5, "min": 1, "max": 50},
                "DEBOUNCE_DELAY": {"default": 30, "min": 0, "max": 200},
            }
        }
        
        self.setup_ui()

    def setup_ui(self):
        main_widget = QWidget()
        self.setCentralWidget(main_widget)
        layout = QVBoxLayout(main_widget)
        
        # Create tab widget
        self.tabs = QTabWidget()
        layout.addWidget(self.tabs)
        
        # Create tabs for each parameter group
        self.config_tabs = {}
        for group_name, params in self.parameters.items():
            tab = ConfigTab({group_name: params})
            self.config_tabs[group_name] = tab
            self.tabs.addTab(tab, group_name)
        
        # Add buttons
        button_layout = QHBoxLayout()
        load_button = QPushButton("Load config.h")
        save_button = QPushButton("Save config.h")
        button_layout.addWidget(load_button)
        button_layout.addWidget(save_button)
        layout.addLayout(button_layout)
        
        load_button.clicked.connect(self.load_config)
        save_button.clicked.connect(self.save_config)

    def load_config(self):
        filename, _ = QFileDialog.getOpenFileName(self, "Open config.h", "", "Header files (*.h)")
        if not filename:
            return
            
        try:
            with open(filename, 'r') as file:
                content = file.read()
            
            # Parse the config file
            params = ConfigParser.parse_config_file(content)
            
            # Update each tab with the loaded values
            for group_name, tab in self.config_tabs.items():
                tab.update_values(params)
            
            QMessageBox.information(self, "Success", "Configuration loaded successfully!")
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Failed to load file: {str(e)}")

    def save_config(self):
        filename, _ = QFileDialog.getSaveFileName(self, "Save config.h", "", "Header files (*.h)")
        if not filename:
            return
            
        try:
            # Collect all parameters
            all_params = {}
            for tab in self.config_tabs.values():
                all_params.update(tab.get_values())
            
            # Generate config.h content
            content = self.generate_config_content(all_params)
            
            with open(filename, 'w') as file:
                file.write(content)
            
            QMessageBox.information(self, "Success", "Configuration saved successfully!")
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Failed to save file: {str(e)}")

    def generate_config_content(self, params: Dict[str, Any]) -> str:
        content = [
            "#ifndef CONFIG_H",
            "#define CONFIG_H",
            "",
            "#include <Arduino.h>",
            "",
            "// Debug Level",
            "#ifndef DEBUG_LEVEL",
            "#define DEBUG_LEVEL 2  // Set to speed mode by default",
            "#endif",
            "",
            "#if DEBUG_LEVEL > 0",
            '#include "DataStructures.h"',
            "#endif",
            ""
        ]
        
        # Add parameters by group
        for group_name, group_params in self.parameters.items():
            content.append(f"// {group_name}")
            for param_name, param_info in group_params.items():
                value = params.get(param_name, param_info['default'])
                if isinstance(value, float):
                    content.append(f"static constexpr float {param_name} = {value}f;")
                else:
                    if param_name in ["SETUP_DELAY", "CALIBRATION_DELAY", "MARKER_READ_INTERVAL", "DEBOUNCE_DELAY"]:
                        content.append(f"static constexpr uint16_t {param_name} = {value};")
                    else:
                        content.append(f"static constexpr uint8_t {param_name} = {value};")
            content.append("")
        
        content.extend([
            "#endif // DEBUG_LEVEL > 0",
            "#endif // CONFIG_H"
        ])
        
        return "\n".join(content)

def main():
    app = QApplication(sys.argv)
    window = ConfigEditor()
    window.show()
    sys.exit(app.exec())

if __name__ == '__main__':
    main()