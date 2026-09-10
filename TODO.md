# Cest TODO (Prático e Executável)

## 0. Bugs encontrados em code review (Clean Code / C-C++/ObjC) — prioridade máxima

- [x] `CEST_THREAD_SAFE` não protegia nada (`cest.h:236-262`)
  - Corrigido: `_cest_mutex` agora é declarado (`CEST_WEAK pthread_mutex_t`, compartilhado entre TUs como `_cest_global_stats`)
  - `CEST_LOCK()`/`CEST_UNLOCK()` agora envolvem toda mutação de `_cest_global_stats` (`_cest_assert_impl`, `b_toBeCloseTo`, `b_toBeInRange`, `CEST_FORK_TEST`, `test()`, skip/only, Valgrind)
  - `_cest_ctx` agora é `CEST_THREAD_LOCAL` (novo macro: `thread_local`/`_Thread_local`/`__declspec(thread)`/`__thread` conforme o compilador) quando `CEST_THREAD_SAFE` está ativo
  - Limite reconhecido: o runner (`describe`/`test`) continua sequencial por design; o que ficou protegido é o caso de teste que sobe threads próprias e chama `expect()` nelas

- [x] Build quebrava no MSVC apesar da doc dizer que é suportado (`cest.h:12-15`)
  - Corrigido: `<unistd.h>` e `<sys/wait.h>` agora só são incluídos dentro de `#ifndef _WIN32`
  - Corrigido: `WEXITSTATUS(ret)` no hook `CEST_POST_RUN` (`cest_result()`) agora tem ramo `#ifdef _WIN32` que usa o `ret` de `system()` diretamente (Windows não decompõe o código de saída via macro)
  - Bônus encontrado ao mexer aqui: `_cest_write_junit`/`_cest_write_json` eram chamadas incondicionalmente em `cest_result()` mas só existiam dentro de `#ifndef CEST_NO_CLI` — quebrava a build com `CEST_NO_CLI` definido. Adicionados stubs no `#else`.

- [x] Dupla avaliação em macro `_CEST_ABS(x)` (`cest.h:288-297`)
  - Corrigido para GCC/Clang via statement-expression + `__typeof__` (avalia `x` uma única vez)
  - MSVC/outros mantêm o fallback antigo (`((x) < 0 ? -(x) : (x))`), documentado como limitação conhecida — não há forma 100% portátil sem C11 genérico por tipo fixo

- [x] Relatórios `--junit`/`--json` incompletos (`cest.h:554-609, 1206-1253`)
  - Corrigido: novo registro por teste (`cest_test_record_t` / `_cest_test_records[]`, capacidade configurável via `CEST_MAX_TEST_RECORDS`) alimentado pelo macro `test()`
  - `--junit` agora emite um `<testcase>` por teste (com `<failure>` quando aplicável); `--json` agora emite a lista `"tests": [...]` com nome/tempo/status
  - Nomes de teste são escapados (XML e JSON) para não gerar relatório malformado

- [x] `system(post_run)` sem aviso de risco (`cest.h:1351-1354`)
  - Corrigido: comentário `SECURITY` explícito acima da chamada, avisando para nunca alimentar `CEST_POST_RUN` com input não confiável

- [x] Exemplo oficial de ObjC liberava memória do jeito errado (`examples/objc/advanced.m:25`)
  - Corrigido: `free(obj)` → `object_dispose(obj)` (par correto de `class_createInstance`)

- [x] Header guard usava identificador reservado (`cest.h:1-2, 1418`)
  - Corrigido: `_CEST_H_` → `CEST_H_INCLUDED` (único uso reservado que era trivial de trocar sem tocar API pública)

- [ ] **Parcial/deferido** — identificadores reservados internos (`_CEST_*`, `_cest_*` em ~250 ocorrências)
  - Não renomeado em massa nesta rodada: `_cest_global_stats` é API pública **documentada** (`docs/*/api.md` e `docs/*/types.md` instruem o usuário a fazer `extern cest_stats_t _cest_global_stats;`), então um rename quebraria código de usuário sem uma estratégia de depreciação (nome novo + alias antigo por uma versão, atualização dos dois idiomas de doc)
  - Não há compilador (gcc/g++/clang/MSVC) disponível neste ambiente para validar um rename mecânico de ~250 ocorrências antes de commitar — risco de regressão silenciosa é alto demais para fazer "às cegas"
  - Próximo passo sugerido: decidir o novo nome público para `_cest_global_stats` (ex.: `cest_stats()` função em vez de variável exportada), depreciar o símbolo antigo, só então renomear o resto internamente com testes de build em gcc/clang/MSVC reais

- [ ] **Parcial/deferido** — poluição de namespace global (`match_eq`, `match_gt`, `match_lt`, `match_contain`, `b_toEqual` etc.)
  - Mesmo bloqueio do item acima: `match_eq`, `match_gt`, `match_lt`, `match_contain` estão documentados em `docs/*/types.md` como "example match functions" do tipo `cest_match_fn` — renomear exige atualizar a doc nos dois idiomas e não pôde ser validado por falta de compilador no ambiente
  - As demais (`match_in_range`, `match_start_with`, `match_end_with`, `match_regex`, `match_truthy`, `match_falsy`, `match_defined`, `match_undefined`, `b_toEqual` e variantes) não estão documentadas e são candidatas seguras para prefixar (`cest_match_*`/`cest_bridge_*`) numa próxima rodada com build real disponível

---

## 1. Segurança básica (ESSENCIAL)

- [x] Garantir que `expect(x)` avalia `x` apenas UMA vez
  - Criar variável temporária interna
  - Testar com: `expect(i++)`

---

## 2. Mensagens de erro úteis

- [x] Melhorar erro de string
  - Mostrar posição da diferença

- [x] Melhorar erro de double
  - Mostrar diff absoluto

---

## 3. Métrica de tempo (mínimo viável)

- [x] Medir tempo por `it()`
  - usar `clock()` (simples, portátil)

- [x] Mostrar no output:
  ✓ test name (0.0001s)

---

## 4. Leak detection simples

- [x] Criar contador global:
  - allocs++
  - frees++

- [x] Macros:
  - `#define malloc(...) cest_malloc(__VA_ARGS__)`
  - `#define free(...) cest_free(__VA_ARGS__)`

- [x] No final:
  - se allocs != frees → warning

- [x] Compatibilidade com sanitizers (ASan, TSan, MSan)
  - Desabilitar leak detection quando sanitizers estão ativos

---

## 5. CLI mínima útil

- [x] Filtro simples:
  - rodar só testes que contenham string

Ex:
  ./test "Math"

---

## 6. Um matcher que realmente falta

- [x] Array básico:
  - `expect_array(ptr, len).toEqual(ptr2, len)`

---

## 7. Testes que FALHAM (sim, você precisa disso)

- [x] Criar exemplos que quebram:
  - string diferente
  - ponteiro errado
  - float impreciso

# CEST TODO — Evolução Prática

## 1. Estabilidade do Core (prioridade máxima)
- [ ] Garantir consistência de tipos em `match_eq` (evitar comportamento implícito estranho)
- [ ] Revisar coerção entre tipos (int vs double vs bool vs ptr)
- [ ] Melhorar mensagens de erro (diff claro entre esperado vs recebido)
- [ ] Garantir que `_cest_ctx` nunca vaze entre asserts
- [ ] Validar comportamento em múltiplas TUs (linkagem weak)
- [ ] Testar com diferentes compiladores (gcc, clang, msvc)

---

## 2. Expansão de Matchers (onde realmente importa)
### Numéricos
- [x] `toBeApprox(expected, epsilon)` (já existe `toBeCloseTo`)
- [x] `toBeInRange(min, max)`
- [ ] `toSatisfy(predicate_fn)`

### Strings
- [x] `toStartWith(str)`
- [x] `toEndWith(str)`
- [ ] `toMatchRegex` (já existe parcial → consolidar)

### Ponteiros / memória
- [ ] `toPointTo(expected_ptr)`
- [ ] `toAlias(ptr)` (mesmo endereço)
- [ ] `toBeAligned(alignment)`

### Containers (C++ apenas)
- [ ] `toContainElement(x)` (vector, list)
- [ ] `toHaveSize(n)`

### Negação
- [ ] `.not` chain (ex: `expect(x).not.toEqual(y)`)

---

## 3. Sistema de Testes mais robusto
- [x] `beforeAll` / `afterAll`
- [ ] `beforeEach` / `afterEach` (já existe)
- [ ] Isolamento de testes (reset de estado global)
- [ ] Melhor suporte a `skip` e `only` (realmente funcional, não decorativo)
- [ ] Filtro de testes por nome (CLI funcional de verdade)
- [ ] Agrupamento de falhas por suite

---

## 4. Benchmark integrado (sem frescura)
- [x] Macro `bench(name, block)`
- [x] Medição simples:
  - tempo total
  - média por iteração
- [ ] Warmup automático
- [ ] Comparação básica entre benchmarks
- [ ] Output separado de testes normais

---

## 5. Memory Leak Detection (sem reinventar malloc… por favor)
### Modo leve (interno)
- [ ] Hook opcional de `malloc/free`
- [ ] Contador de alocações vs frees
- [ ] Report simples no final

### Integração externa (o caminho certo)
- [ ] Suporte a:
  - AddressSanitizer (ASan)
  - LeakSanitizer (LSan)
  - UndefinedBehaviorSanitizer (UBSan)
- [ ] Flags automáticas via macro:
  - `CEST_ENABLE_SANITIZERS`

---

## 6. Snapshot de erro (pra parar de sofrer debugando)
- [ ] Capturar estado do assert (valores + tipo)
- [ ] Salvar snapshot opcional em arquivo
- [ ] Comparação futura (tipo snapshot testing simples)
- [ ] Dump de memória opcional (hex)

---

## 7. Suporte a Freestanding / Low-level
- [ ] Flag `CEST_FREESTANDING`
- [ ] Remover dependências:
  - stdio (ou abstrair)
  - malloc (opcional)
- [ ] Permitir output custom (callback)
- [ ] Rodar em:
  - kernel space (limitado)
  - embedded

---

## 8. CLI decente (não esse parse improvisado)
- [ ] `--filter <pattern>`
- [ ] `--list`
- [ ] `--fail-fast`
- [ ] `--bench-only`
- [ ] `--json-output` (integração CI)

---

## 9. Output estruturado
- [ ] JSON reporter
- [ ] TAP (Test Anything Protocol)
- [ ] Modo silencioso (CI-friendly)

---

## 10. Testes do próprio CEST (sim, você precisa testar seu test framework)
- [ ] Self-tests cobrindo:
  - todos os matchers
  - edge cases (NULL, NaN, overflow)
- [ ] Testes cruzados C / C++ / ObjC
- [ ] Testes com sanitizers ativados
- [ ] Testes multi-thread (se habilitado)

---

## 11. Organização interna
- [ ] Separar core vs extensões (header modular opcional)
- [ ] Reduzir macros perigosas
- [ ] Melhorar legibilidade do fluxo expect → matcher

---

## 12. Nice-to-have (não se perca aqui)
- [ ] Color themes
- [ ] Output compacto vs verboso
- [ ] Integração com CI (GitHub Actions exemplo)
