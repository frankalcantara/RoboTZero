# 🧪 Diretório: /tests

## Propósito

Este diretório contém todos os testes automatizados para garantir que o código no diretório `/src` funciona corretamente.

## Arquivos Esperados

* **Testes Unitários:** Scripts Python (ex: `test_ssl.py`, `test_vad.py`) que usam `pytest` ou `unittest` para verificar pequenas partes do código.
* **Testes de Integração:** Testes que verificam se o pipeline completo (C++ -> Python) está funcionando.
* **Dados de Teste:** O subdiretório `/audio_samples` contém arquivos de áudio (`.wav`) gravados em posições conhecidas (ex: `30_graus_esquerda.wav`) para validar a precisão dos algoritmos de SSL.
