# 🤖 AuralFocus: Cabeça Robótica com Localização de Som

Este é o repositório principal do projeto AuralFocus. O objetivo é criar uma placa HAT para o Raspberry Pi com um array de 6 microfones, capaz de identificar a origem de um som e direcionar uma cabeça (animada ou robótica) para essa fonte.

## 🎯 Objetivos do Projeto

* **Hardware (HAT):** Desenvolver uma placa de circuito impresso com 6 microfones e circuitos de interface para o Raspberry Pi.
* **Software (SSL):** Implementar algoritmos de Localização de Fonte Sonora (Sound Source Localization - SSL) em tempo real, como GCC-PHAT ou SRP-PHAT.
* **Fase 1 (Animação):** Criar uma aplicação gráfica que exibe uma "cabeça" virtual que se vira para a fonte do som.
* **Fase 2 (Robótica):** Controlar servomotores (pan/tilt) para mover uma cabeça robótica física na direção do som.

## 🚀 Começo Rápido

1.  **Hardware:** Monte a HAT AuralFocus (veja as instruções em `docs/hardware_setup.md`).
2.  **Instalação:** Execute o script de instalação de dependências: `bash scripts/install_deps.sh`.
3.  **Calibração:** Execute o script de calibração: `python scripts/calibrate.py`.
4.  **Rodar:** Inicie uma das aplicações, por exemplo: `python applications/phase1_animation/main_animation.py`.

## 🗺️ Navegação do Repositório

* `/hardware`: Contém todo o design da placa (KiCad, Gerbers, modelos 3D).
* `/src`: Contém o código-fonte da biblioteca principal (processamento de sinal em C++, bindings Python).
* `/applications`: Contém as aplicações executáveis (Fase 1 e Fase 2).
* `/docs`: Documentação detalhada do projeto.
* `/scripts`: Scripts utilitários (instalação, calibração).
* `/tests`: Testes unitários e de integração.
