---
name: devops-engineer
description: Automate deployments and infrastructure with Docker and CI/CD. Use for containerizing applications, building pipelines, orchestrating services, and implementing monitoring.
---

# DevOps Engineer

## Description
A skill for containerization, CI/CD automation, deployment workflows, and runtime operations with Docker, Docker Compose, and GitHub Actions.

## Priority Rules
Prioritize in this order when trade-offs conflict:
1. Security and secret management
2. Reliability and observability
3. Automation and repeatability
4. Efficiency and cost optimization

## When to Use
- Containerizing applications with Docker
- Running multi-service environments via Docker Compose
- Building CI/CD pipelines in GitHub Actions
- Automating build, test, and deployment stages
- Implementing monitoring and centralized logging
- Hardening infrastructure and secret handling

## Instructions
1. **Containerize app** - write an efficient Dockerfile
2. **Define Compose stack** - services, networks, volumes, env files
3. **Create GitHub workflows** - lint, test, build, deploy
4. **Design CI/CD pipeline** - image build, registry push, rollout
5. **Optimize images** - multi-stage builds and cache strategy
6. **Add observability** - metrics, logs, traces, alerting
7. **Secure delivery** - secret management and image scanning
8. **Document operations** - runbooks, rollback, incident flow

## Input Recovery Rules
- Assume standard container best practices when specific environment details are not provided
- Ask for clarification only when deployment target or CI/CD platform is completely undefined

## Constraints
- Do not use `latest` tag for images in production
- Do not run containers as root without explicit justification
- Do not hardcode secrets in Dockerfiles or CI/CD configs

## Docker
- Dockerfile best practices
- Multi-stage builds
- Networks and volumes
- Health checks and environment configuration
- Registry workflows (Docker Hub, GHCR, ECR, GCR)

## Docker Compose
- `docker-compose.yml` services
- Network and volume topology
- `.env` and override files
- Profiles for optional components

## GitHub Actions
- Workflow triggers and job graph
- Reusable actions and matrix builds
- Dependency caching
- Secrets and environment protection

## Typical CI/CD Flow
1. Trigger on push/PR
2. Build and test
3. Lint and security scan
4. Build/push image
5. Deploy to staging/production
6. Run smoke checks
7. Notify team channels

## Monitoring and Security
- Prometheus, Grafana, ELK, Alertmanager
- Trivy or Snyk image scans
- Least privilege runtime
- Immutable and versioned images
- Regular patch cadence

## Best Practices
- Use explicit image versions (avoid `latest`)
- Keep containers non-root where possible
- Use `.dockerignore`
- Log to stdout/stderr
- Validate rollback strategy regularly
