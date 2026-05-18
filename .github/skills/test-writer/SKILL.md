---
name: test-writer
description: Write automated tests for unit, integration, API, and e2e levels. Use for test-driven development, regression coverage, CI/CD integration, and quality assurance.
---

# Test Writer (Test Developer / QA Automation)

## Description
A skill for writing automated tests across unit, integration, API, and end-to-end levels, with CI/CD integration and reliable test design.

## Priority Rules
Prioritize in this order when trade-offs conflict:
1. Test reliability and determinism
2. Test independence and isolation
3. Coverage and risk reduction
4. Maintainability and readability

## When to Use
- Writing unit tests for business logic
- Building integration tests between services/components
- Creating end-to-end flows for user journeys
- Validating APIs and contracts
- Automating regression testing
- Improving and enforcing coverage goals

## Instructions
1. **Choose framework** - align with the project stack
2. **Structure tests** - Arrange, Act, Assert
3. **Write unit tests** - deterministic and isolated
4. **Use fixtures/mocks** - control dependencies and side effects
5. **Write integration tests** - verify interaction boundaries
6. **Add e2e tests** - cover core user scenarios
7. **Keep tests independent** - no hidden coupling
8. **Integrate in CI/CD** - run automatically on pull requests
9. **Track coverage** - use metrics to guide improvements

## Input Recovery Rules
- Assume standard testing patterns for the language when framework is not specified
- Ask for clarification only when test scope or architecture significantly affects test design

## Constraints
- Do not write tests that depend on execution order
- Do not skip mocking external dependencies
- Do not leave failing tests uninvestigated

## Testing Frameworks
- JavaScript/TypeScript: Jest, Mocha, Vitest, Cypress, Playwright
- Python: pytest, unittest
- Java: JUnit, TestNG, Mockito
- C++: GoogleTest, Catch2, Boost.Test
- PHP: PHPUnit, Codeception
- C#: NUnit, xUnit, Moq

## Best Practices
- Prefer clear AAA structure
- Use descriptive test names
- Keep tests fast and deterministic
- Mock external dependencies deliberately
- Include boundary and negative tests
- Separate unit/integration/e2e layers
- Maintain readable failure messages
- Refactor tests as code evolves
