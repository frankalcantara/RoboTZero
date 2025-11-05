import sys
import re
from typing import Dict, Any
from dataclasses import dataclass
from pathlib import Path
from PyQt5.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, 
                           QHBoxLayout, QLabel, QLineEdit, QPushButton, 
                           QTabWidget, QGridLayout, QMessageBox, QFrame)
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QDoubleValidator, QIntValidator

@dataclass
class ConfigParameter:
    name: str
    value: Any
    tooltip: str
    min_val: Any = None
    max_val: Any = None
    category: str = "General"

class TooltipLabel(QLabel):
    def __init__(self, text: str, tooltip: str):
        super().__init__(text)
        self.setToolTip(tooltip)
        self.setMouseTracking(True)
        self.setStyleSheet("""
            QLabel {
                padding: 5px;
            }
            QToolTip {
                background-color: #ffffe0;
                border: 1px solid gray;
                padding: 5px;
                font-size: 12px;
            }
        """)

class ParameterWidget(QFrame):
    def __init__(self, param: ConfigParameter, parent=None):
        super().__init__(parent)
        self.param = param
        
        layout = QHBoxLayout()
        self.setLayout(layout)
        
        # Label com tooltip
        label = TooltipLabel(f"{param.name}:", param.tooltip)
        layout.addWidget(label)
        
        # Campo de entrada
        self.entry = QLineEdit()
        self.entry.setText(str(param.value))
        self.entry.setMaximumWidth(100)
        
        # Configurar validador
        if isinstance(param.value, float):
            validator = QDoubleValidator(param.min_val, param.max_val, 2)
            validator.setNotation(QDoubleValidator.StandardNotation)
        else:
            validator = QIntValidator(param.min_val, param.max_val)
        self.entry.setValidator(validator)
        
        layout.addWidget(self.entry)
        
        # Range label
        if param.min_val is not None and param.max_val is not None:
            range_label = QLabel(f"Faixa: {param.min_val} - {param.max_val}")
            layout.addWidget(range_label)
        
        layout.addStretch()
        
        # Estilo
        self.setStyleSheet("""
            QFrame {
                background-color: #f0f0f0;
                border-radius: 5px;
                margin: 2px;
            }
            QLineEdit {
                padding: 3px;
                border: 1px solid #ccc;
                border-radius: 3px;
            }
        """)

class RobotConfigGUI(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Configuração do Robô")
        self.setGeometry(100, 100, 800, 600)
        
        # Widget central
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QVBoxLayout(central_widget)
        
        # Define os parâmetros
        self.parameters: Dict[str, ConfigParameter] = {
            # Velocidades
            "VELOCIDADE_1": ConfigParameter(
                "VELOCIDADE_1", 60,
                "Velocidade inicial do robô. Aumente se o robô não iniciar movimento.",
                25, 75, "Velocidades"
            ),
            "VELOCIDADE_2": ConfigParameter(
                "VELOCIDADE_2", 100,
                "Velocidade para curvas suaves. Ajuste para melhor desempenho em curvas graduais.",
                75, 125, "Velocidades"
            ),
            "VELOCIDADE_3": ConfigParameter(
                "VELOCIDADE_3", 140,
                "Velocidade de frenagem. Aumente se o robô parar muito bruscamente.",
                120, 160, "Velocidades"
            ),
            "VELOCIDADE_4": ConfigParameter(
                "VELOCIDADE_4", 170,
                "Velocidade média para retas. Ajuste baseado nas características da pista.",
                150, 190, "Velocidades"
            ),
            "VELOCIDADE_5": ConfigParameter(
                "VELOCIDADE_5", 200,
                "Modo lento - Aumente se o robô estiver muito lento.",
                170, 220, "Velocidades"
            ),

            # Parâmetros de Controle
            "ACEL_STEP": ConfigParameter(
                "ACEL_STEP", 25,
                "Passo de aceleração. Aumentar para saída mais rápida de curvas.",
                20, 40, "Controle"
            ),
            "FREIO_STEP": ConfigParameter(
                "FREIO_STEP", 35,
                "Passo de frenagem. Aumentar para frenagem mais forte nas curvas.",
                30, 50, "Controle"
            ),
            "CURVA_VEL": ConfigParameter(
                "CURVA_VEL", 120,
                "Velocidade em curvas. Diminuir se derrapar nas curvas.",
                100, 140, "Controle"
            ),
            "CURVA_ERRO": ConfigParameter(
                "CURVA_ERRO", 45,
                "Limiar de erro para detecção de curvas. Aumentar se detectar curvas muito cedo.",
                40, 60, "Controle"
            ),

            # PID
            "KPROP_DEFAULT": ConfigParameter(
                "KPROP_DEFAULT", 7.0,
                "Ganho proporcional do PID. Aumentar para correção mais agressiva.",
                5.0, 9.0, "PID"
            ),
            "KDERIV_DEFAULT": ConfigParameter(
                "KDERIV_DEFAULT", 300.0,
                "Ganho derivativo do PID. Diminuir se oscilar muito nas retas.",
                200.0, 400.0, "PID"
            ),
            "ALPHA_DEFAULT": ConfigParameter(
                "ALPHA_DEFAULT", 0.7,
                "Fator de suavização do filtro. Aumentar para resposta mais rápida.",
                0.6, 0.9, "PID"
            ),

            # Sensores
            "SENSOR_THRESHOLD": ConfigParameter(
                "SENSOR_THRESHOLD", 120,
                "Limiar dos sensores. Aumentar se não detectar a linha adequadamente.",
                100, 150, "Sensores"
            ),
            "PESO_S1": ConfigParameter(
                "PESO_S1", -1.5,
                "Peso do sensor extremo esquerdo. Aumentar para correções mais agressivas.",
                -1.8, -1.2, "Sensores"
            ),
            "PESO_S6": ConfigParameter(
                "PESO_S6", 1.5,
                "Peso do sensor extremo direito. Aumentar para correções mais agressivas.",
                1.2, 1.8, "Sensores"
            )
        }
        
        # Criar tabs
        self.tab_widget = QTabWidget()
        main_layout.addWidget(self.tab_widget)
        
        # Criar widgets para cada categoria
        self.parameter_widgets = {}
        self.create_tabs()
        
        # Botões de ação
        button_layout = QHBoxLayout()
        
        save_button = QPushButton("Salvar")
        save_button.clicked.connect(self.save_config)
        button_layout.addWidget(save_button)
        
        reload_button = QPushButton("Recarregar")
        reload_button.clicked.connect(self.load_config)
        button_layout.addWidget(reload_button)
        
        default_button = QPushButton("Restaurar Padrões")
        default_button.clicked.connect(self.restore_defaults)
        button_layout.addWidget(default_button)
        
        main_layout.addLayout(button_layout)
        
        # Status bar
        self.statusBar().showMessage("Pronto")
        
        # Carregar configuração inicial
        self.load_config()
        
        # Estilo global
        self.setStyleSheet("""
            QMainWindow {
                background-color: white;
            }
            QPushButton {
                padding: 5px 15px;
                background-color: #4CAF50;
                color: white;
                border: none;
                border-radius: 3px;
            }
            QPushButton:hover {
                background-color: #45a049;
            }
            QTabWidget::pane {
                border: 1px solid #ccc;
                background: white;
            }
            QTabWidget::tab-bar {
                left: 5px;
            }
            QTabBar::tab {
                background: #f0f0f0;
                border: 1px solid #ccc;
                padding: 5px 10px;
                margin-right: 2px;
            }
            QTabBar::tab:selected {
                background: white;
            }
        """)

    def create_tabs(self):
        categories = set(param.category for param in self.parameters.values())
        for category in categories:
            tab = QWidget()
            layout = QVBoxLayout(tab)
            
            # Adicionar parâmetros desta categoria
            for param_name, param in self.parameters.items():
                if param.category == category:
                    widget = ParameterWidget(param)
                    self.parameter_widgets[param_name] = widget
                    layout.addWidget(widget)
            
            layout.addStretch()
            self.tab_widget.addTab(tab, category)

    def load_config(self):
        try:
            with open('config.h', 'r') as file:
                content = file.read()
                for param_name, widget in self.parameter_widgets.items():
                    pattern = fr'#define\s+{param_name}\s+([^/\n]+)'
                    match = re.search(pattern, content)
                    if match:
                        value = match.group(1).strip()
                        widget.entry.setText(value)
            self.statusBar().showMessage("Configuração carregada com sucesso", 3000)
        except FileNotFoundError:
            QMessageBox.warning(self, "Aviso", "Arquivo config.h não encontrado!")
            self.statusBar().showMessage("Erro ao carregar configuração", 3000)

    def save_config(self):
        try:
            with open('config.h', 'r') as file:
                content = file.read()

            for param_name, widget in self.parameter_widgets.items():
                new_value = widget.entry.text()
                param = self.parameters[param_name]
                
                # Atualizar o valor no arquivo
                pattern = fr'(#define\s+{param_name}\s+)[^/\n]+'
                content = re.sub(pattern, fr'\g<1>{new_value}', content)

            with open('config.h', 'w') as file:
                file.write(content)
            
            self.statusBar().showMessage("Configuração salva com sucesso", 3000)

        except Exception as e:
            QMessageBox.critical(self, "Erro", f"Erro ao salvar: {str(e)}")
            self.statusBar().showMessage("Erro ao salvar configuração", 3000)

    def restore_defaults(self):
        reply = QMessageBox.question(
            self, 'Confirmar',
            "Deseja restaurar todos os valores para o padrão?",
            QMessageBox.Yes | QMessageBox.No, QMessageBox.No
        )
        
        if reply == QMessageBox.Yes:
            for param_name, widget in self.parameter_widgets.items():
                param = self.parameters[param_name]
                widget.entry.setText(str(param.value))
            self.statusBar().showMessage("Valores padrão restaurados", 3000)

def main():
    app = QApplication(sys.argv)
    window = RobotConfigGUI()
    window.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()