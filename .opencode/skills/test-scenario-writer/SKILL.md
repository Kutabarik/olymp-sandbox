---
name: test-scenario-writer
description: Design BDD test scenarios in Gherkin/Cucumber from requirements. Use for acceptance criteria formalization, behavior coverage planning, and traceable executable scenarios.
---

# Test Scenario Writer (QA/Test Design)

## Description
A skill for creating comprehensive BDD scenarios in Gherkin/Cucumber from requirements, including happy paths, edge cases, and risk-based behavior coverage.

## Priority Rules
Prioritize in this order when trade-offs conflict. If a broader edge-case set would weaken traceability to explicit requirements, keep only the edge cases supported by stated requirements or directly implied boundary conditions. If execution prioritization conflicts with scenario clarity, preserve clarity first and defer prioritization to the final ranking step:
1. Requirement traceability and coverage completeness
2. Edge case and negative scenario identification
3. Gherkin scenario clarity and executability
4. Risk-based execution prioritization

## When to Use
- Building BDD scenarios for new features
- Formalizing acceptance criteria in Given/When/Then format
- Estimating and improving behavior coverage
- Capturing edge and negative behavior cases
- Preparing tagged regression and smoke suites
- Aligning business-readable scenarios with automation

## Instructions
1. **Review requirements** - specs, stories, and acceptance criteria
2. **Map user flows** - primary and alternative paths
3. **Identify edge cases** - invalid input and boundary values
4. **Prepare test data** - valid, invalid, and stateful data sets
5. **Plan coverage** - prioritize by risk and business impact
6. **Write Gherkin scenarios** - use Feature, Scenario, Given/When/Then, and Scenario Outline where needed
7. **Apply tagging and traceability** - add tags like @req-id, @smoke, @regression, @negative
8. **Prioritize execution** - critical, high, medium, low
9. **Review quality** - completeness, traceability, maintainability, and step reusability

## Input Recovery Rules
- Infer test scope only from explicitly stated requirements and directly implied boundary conditions when specs are incomplete
- Ask for clarification only when requirements ambiguity affects behavior coverage or Given/When/Then correctness

## Constraints
- Do not create scenarios without traceable requirements
- Do not skip negative and boundary value scenarios
- Do not document ambiguous Then outcomes
- Do not encode UI implementation details in Gherkin steps unless explicitly requested

## Output Contract
- Default output format is Gherkin/Cucumber
- Start with a `Feature:` block describing business behavior
- Use `Scenario:` for single-path behavior and `Scenario Outline:` with `Examples:` for data-driven variants
- Keep one business behavior outcome per scenario
- Use Given/When/Then language that is understandable by non-technical stakeholders
- Add tags for traceability and execution sets (for example, `@req-123`, `@smoke`, `@regression`, `@negative`)

## Definition of Done
- Every scenario maps to at least one explicit requirement or acceptance criterion
- Steps are reusable and avoid duplicated phrasing for equivalent actions
- Scenario intent is clear to business and QA audiences
- Outcomes are testable and unambiguous in Then clauses
- Coverage includes happy path, negative path, and key boundary behavior

## Examples

### Canonical Feature and Scenario

```gherkin
@req-101 @smoke
Feature: User authentication
	As a registered user
	I want to sign in with valid credentials
	So that I can access my account

	Scenario: Successful sign in with valid credentials
		Given a registered user exists
		And the user is on the sign-in page
		When the user submits valid email and password
		Then access is granted
		And the user sees the account dashboard
```

### Canonical Scenario Outline

```gherkin
@req-102 @negative @regression
Feature: Sign-in input validation

	Scenario Outline: Sign in fails for invalid credentials
		Given a registered user exists
		And the user is on the sign-in page
		When the user submits email <email> and password <password>
		Then access is denied
		And the user sees error message <error_message>

		Examples:
			| email            | password  | error_message                 |
			| user@example.com | wrong123  | Invalid email or password     |
			| bad-email        | valid123  | Enter a valid email address   |
			|                  | valid123  | Email is required             |
```

## Methods and Approaches
- Behavior-Driven Development (BDD)
- Gherkin/Cucumber specification-by-example
- Black-box testing
- Boundary Value Analysis
- Equivalence Partitioning
- Decision Table Testing
- State Transition Testing
- Exploratory Testing
- Risk-based Testing
- Regression strategy and maintenance
- For non-functional testing (performance, security, reliability), define complementary test artifacts outside Gherkin scenarios
