# CI/CD Integration Guide

Cest integrates seamlessly with continuous integration and deployment platforms through multiple output formats.

## Output Formats

Cest supports three output modes:

| Format | Use Case | Flag |
|--------|----------|------|
| **Terminal** | Local development, human-readable | (default) |
| **JUnit XML** | Jenkins, GitHub Actions, GitLab CI | `--junit` |
| **JSON** | Custom tooling, data processing | `--json` |

---

## JUnit XML Output

### Basic Usage

```bash
# Compile your test
gcc -o test examples/ci_integration.c

# Run with JUnit output
./test --junit report.xml
```

### Output Example

```xml
<?xml version="1.0" encoding="UTF-8"?>
<testsuites>
  <testsuite name="Calculator Functions" tests="5" failures="0" time="0.001">
    <testcase name="adds two positive numbers" time="0.0001"/>
    <testcase name="adds negative numbers" time="0.0001"/>
    <testcase name="subtracts numbers" time="0.0001"/>
    <testcase name="multiplies numbers" time="0.0001"/>
    <testcase name="handles zero" time="0.0001"/>
  </testsuite>
  <testsuite name="String Functions" tests="5" failures="0" time="0.002">
    <testcase name="compares equal strings" time="0.0001"/>
    ...
  </testsuite>
</testsuites>
```

---

## JSON Output

### Basic Usage

```bash
./test --json report.json
```

### Output Example

```json
{
  "suites": [
    {
      "name": "Calculator Functions",
      "tests": 5,
      "passed": 5,
      "failed": 0,
      "skipped": 0,
      "pending": 0,
      "testcases": [
        {
          "name": "adds two positive numbers",
          "status": "passed",
          "time_ms": 0.1
        }
      ]
    }
  ],
  "summary": {
    "total": 25,
    "passed": 25,
    "failed": 0
  }
}
```

---

## GitHub Actions Integration

### Workflow Example

Create `.github/workflows/tests.yml`:

```yaml
name: C Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    
    steps:
      - uses: actions/checkout@v3
      
      - name: Install dependencies
        run: sudo apt-get update && sudo apt-get install -y build-essential
      
      - name: Build tests
        run: make
      
      - name: Run tests with JUnit output
        run: ./build/basic_c --junit test-results.xml
        continue-on-error: true
      
      - name: Publish test results
        if: always()
        uses: dorny/test-reporter@v1
        with:
          name: C Tests
          path: test-results.xml
          reporter: 'java-junit'
```

### Features

- ✅ Automatic test result parsing
- ✅ Test summary in workflow logs
- ✅ Annotated source code with failures
- ✅ PR comments with test status

---

## GitLab CI Integration

### `.gitlab-ci.yml` Example

```yaml
stages:
  - test

test:c:
  stage: test
  image: ubuntu:latest
  script:
    - apt-get update
    - apt-get install -y build-essential
    - make
    - ./build/basic_c --junit report.xml
  artifacts:
    reports:
      junit: report.xml
    expire_in: 1 week
  allow_failure: false
```

### Features

- ✅ Automatic test result parsing
- ✅ Pipeline status badges
- ✅ Test history tracking

---

## Jenkins Integration

### Jenkinsfile Example

```groovy
pipeline {
    agent any
    
    stages {
        stage('Build') {
            steps {
                sh 'make clean'
                sh 'make'
            }
        }
        
        stage('Test') {
            steps {
                sh './build/basic_c --junit test-results.xml'
            }
        }
    }
    
    post {
        always {
            junit 'test-results.xml'
        }
        
        unstable {
            emailext(
                subject: 'Build ${BUILD_NUMBER} - Test Failures',
                body: 'Some tests failed. Check the console output.',
                to: '${DEFAULT_RECIPIENTS}'
            )
        }
    }
}
```

### Features

- ✅ Test result trending
- ✅ Email notifications
- ✅ Build artifacts storage

---

## Combining Outputs

Run tests with both output formats simultaneously:

```bash
./test --junit junit.xml --json report.json
```

This allows:
- Parsing results with multiple tools
- Different formats for different systems
- Post-processing with custom scripts

---

## With Sanitizers

Pass sanitizer flags and test output together:

```bash
# Compile with AddressSanitizer
gcc -g -fsanitize=address -o test examples/leak_detection.c

# Run tests with JUnit output (sanitizer warnings still output to stderr)
./test --junit test-results.xml 2>&1 | tee stderr.log
```

---

## With Valgrind

For memory leak detection using Valgrind:

```bash
# Compile without sanitizers for Valgrind
gcc -g -o test examples/leak_detection.c

# Run with Valgrind
valgrind --leak-check=full --gen-suppressions=all \
         ./test --junit test-results.xml
```

---

## Filtering Tests

Run specific tests for CI pipelines:

```bash
# Run only tests matching pattern (requires CLI support)
./test ".*String.*"

# Compile with skip/only support
gcc -DCEST_ENABLE_SKIP -o test examples/skip_only.c
./test --junit test-results.xml
```

---

## Color Output Control

Control colored output based on environment:

```bash
# Disable colors for CI (usually needed)
CEST_NO_COLORS=1 ./test --junit report.xml

# Or with environment detection (automatic)
./test --junit report.xml  # Colors disabled if not TTY
```

---

## Exit Codes

Cest follows standard exit code conventions:

| Exit Code | Meaning |
|-----------|---------|
| `0` | All tests passed |
| `1` | One or more tests failed |
| `2` | Runtime error or invalid arguments |

### Note for IDEs and CI Tools

```bash
#!/bin/bash
./test --junit report.xml
exit_code=$?

# Process report.xml
process_results report.xml

exit $exit_code  # Propagate test status
```

---

## Dockerfile Example

```dockerfile
FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    valgrind \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN make

# Run tests with all outputs
CMD make test --junit /output/junit.xml && \
    ./build/basic_c --json /output/report.json
```

---

## GitHub Actions - Complete Example

```yaml
name: C Test Suite

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  test:
    name: Run Tests
    runs-on: ubuntu-latest
    
    strategy:
      matrix:
        compiler: [gcc, clang]
    
    steps:
      - uses: actions/checkout@v3
      
      - name: Install dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y \
            build-essential \
            clang \
            libobjc-dev \
            libasan5 \
            libubsan1
      
      - name: Set compiler
        run: export CC=${{ matrix.compiler }}
      
      - name: Build tests
        run: make clean && make
      
      - name: Run tests
        run: ./build/basic_c --junit test-results.xml --json report.json
        continue-on-error: true
      
      - name: Upload results
        if: always()
        uses: actions/upload-artifact@v3
        with:
          name: test-results-${{ matrix.compiler }}
          path: |
            test-results.xml
            report.json
      
      - name: Report results
        if: always()
        uses: dorny/test-reporter@v1
        with:
          name: Tests (${{ matrix.compiler }})
          path: test-results.xml
          reporter: 'java-junit'
          fail-on-error: true
```

---

## Best Practices

1. **Always use parallel execution** - Run different test suites in parallel for faster feedback
2. **Archive artifacts** - Keep test reports for historical analysis
3. **Fail fast** - Break CI on first failure for immediate feedback
4. **Test both compilers** - Test with gcc and clang in CI
5. **Run with sanitizers** - Enable ASan in CI for memory safety
6. **Check exit codes** - Don't rely solely on output parsing
7. **Separate unit and integration tests** - Different timeouts and resources

---

## See Also

- [Examples - CI Integration](../examples/ci_integration.c)
- [API Reference](./api.md)
- [Installation Guide](./installation.md)
