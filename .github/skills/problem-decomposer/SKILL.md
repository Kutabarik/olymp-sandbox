---
name: problem-decomposer
description: Break complex problems into manageable parts. Use for large or vague tasks, multi-team projects, phased implementation plans, risk isolation, and clear handoff points.
---

# Problem Decomposer

## Description
A general-purpose skill for breaking complex problems into manageable parts with clear scope, dependencies, and execution order.

## Priority Rules
Prioritize in this order when trade-offs conflict:
1. Clear scope definition first
2. Dependency identification and ordering
3. Risk isolation and mitigation
4. Resource and timeline constraints

## When to Use
- A task is too large or vague to execute directly
- Multiple teams or domains are involved
- You need a phased implementation plan
- Risks and unknowns must be isolated early
- You need clear handoff points between steps

## Instructions
1. **Define problem statement** - objective (what the problem aims to achieve), boundaries (specific limits such as time, scope, or resources), and expected outcome (clear deliverables or results)
2. **Map constraints** - map time constraints first, then resource constraints, then dependencies, and finally non-negotiables
3. **Split into subproblems** - independent chunks with explicit inputs/outputs
4. **Order execution** - sequence by dependency and risk reduction
5. **Identify checkpoints** - measurable milestones and review gates
6. **Flag risks and unknowns** - assumptions, blockers, and validation tasks
7. **Produce action plan** - owners, priority, and next concrete steps

## Input Recovery Rules
- Infer reasonable boundaries from the task context when constraints are not explicitly stated
- Ask for clarification only when the objective, scope, or stakeholders are completely undefined

## Constraints
- Do not produce subproblems without clear inputs and outputs
- Do not skip dependency ordering between subproblems
- Do not assume unlimited resources or timeline

## Tools and Methods
- Work breakdown structure (WBS)
- Dependency mapping
- Critical path thinking
- Milestone planning
- Risk register

## Output Contract
Return all of the following:
1. Problem statement and boundaries
2. Decomposed subproblems with inputs and outputs
3. Dependency-aware execution order
4. Risks, unknowns, and checkpoints
5. Concrete action plan with next steps

## Best Practices
- Decompose by dependency and validation value, not only by topic
- Keep subproblems small enough to assign and verify independently
- Surface assumptions and blockers early instead of embedding them in the plan
- Use milestones that can falsify the current plan before major downstream work
