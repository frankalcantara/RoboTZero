# 🧠 Diretório: /src (Source)

## Propósito

Este diretório contém o "cérebro" do projeto: o código-fonte da biblioteca principal. Este código é compilado e instalado como um pacote (ex: um módulo Python) para ser *usado* pelas aplicações.

O código aqui deve ser reutilizável e não deve conter lógica de aplicação específica.

## Arquivos Esperados

* **Processamento de Sinal (C++):** O subdiretório `/core_processing` contém o código de alta performance (`.cpp`, `.h`) para os algoritmos de SSL (ex: FFT, GCC-PHAT) que rodam rápido no RPi.
* **Bindings (Python/C++):** O subdiretório `/python_bindings` contém o código "cola" (ex: usando `pybind11`) que expõe as funções C++ para o Python.
* **Arquivos de Build:** `CMakeLists.txt` (para C++) e `setup.py` (para criar o pacote Python).
