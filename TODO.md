# Cest TODO (Prático e Executável)

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
