# Evidências de interoperabilidade

Este diretório guarda **evidências**, não implementação. O objetivo é permitir
que uma especificação seja escrita e auditada sem que o implementador precise
observar diretamente um produto proprietário.

## Regras

1. Só colete evidência de alvos que você está autorizado a examinar.
2. Leia os termos de uso/EULA antes; registre a conclusão.
3. Não contorne autenticação, paywall, rate limit ou proteção técnica.
4. Não copie código, CSS, HTML, modelos 3D, imagens, fontes ou dados.
5. Não registre tokens, cookies, headers de autorização ou dados pessoais.
6. Use um perfil de navegador isolado e uma conta dedicada.
7. Toda conclusão precisa distinguir observação, derivação, inferência e
   desconhecido.
8. A spec resultante não pode conter trechos de código proprietário.
9. O `clean-room-auditor` revisa antes de qualquer implementação.

## Estrutura

```text
evidence/
  2026-10-22/
    wokwi-project-schema/
      README.md          # ficha da evidência
      capture.json       # saída bruta do REA
      capture.sha256     # hash do arquivo
      screenshot.png     # opcional
      notes.md           # análise e limitações
```

## Ficha de evidência

```markdown
# <tema>

## Pergunta
O que queremos responder?

## Alvo
- Produto/versão:
- URL/origem:
- Data/hora:
- Ferramenta/versão:
- Base legal/termos:

## Hipótese
O que esperávamos encontrar?

## Comando/procedimento
Comandos exatos ou passos manuais.

## Observado
Fatos objetivos, sem interpretação.

## Derivado
O que os fatos permitem concluir com lógica.

## Inferido
Hipóteses plausíveis que ainda precisam de teste.

## Desconhecido
O que não foi possível determinar.

## Limitações
Cobertura, janela de captura, minificação, redaction, etc.

## Implicações para o projeto
O que isso muda na spec/arquitetura.

## Licença/clean-room
- Há material proprietário? (sim/não)
- Foi copiado para o repositório? (não)
- Precisa de revisão? (sim/não)
- Revisado por:
```

## Níveis de confiança

| Nível | Significado | Pode virar requisito? |
| --- | --- | --- |
| **Observado** | Visto diretamente em captura/log | Sim, com citação |
| **Observado (documental)** | Afirmado por documentação pública licenciada; não executado nem visto em runtime | Não sozinho; exige spec e teste |
| **Derivado** | Dedução direta de fatos observados | Sim, com justificativa |
| **Inferido** | Hipótese plausível | Não; precisa de teste |
| **Desconhecido** | Não determinado | Não; registrar como unknown |

## Redaction

Antes de salvar qualquer captura:

- remover `Authorization`, `Cookie`, `Set-Cookie`, tokens e query strings
  sensíveis;
- remover dados pessoais;
- truncar corpos binários grandes;
- substituir IDs de conta por hashes;
- revisar screenshots (não capturar dados privados).

## Hash

```powershell
Get-FileHash -Algorithm SHA256 capture.json | Format-List
```

Registre o hash no `README.md` da evidência e no commit que a usar.

## Relação com specs

Uma spec em `docs/` deve citar:

```text
Fonte: evidence/2026-10-22/wokwi-project-schema/README.md
Nível: derivado
Limitação: bundle minificado, sem source map
```

Se a spec não puder citar uma evidência, ela é uma hipótese e deve ser marcada
como tal.

## O que não entra aqui

- Código-fonte proprietário.
- Assets proprietários.
- Firmware sem licença.
- Credenciais.
- Dados de usuários.
- Conteúdo de paywall/área privada.
