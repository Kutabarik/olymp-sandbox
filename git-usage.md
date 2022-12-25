# GIT usage

Посмотреть существующие ветки:

```
git branch
```

Создать ветку `my-cool-branch`:

```
git branch my-cool-branch 
```

Перейти на ветку `my-cool-branch`:

```
git checkout my-cool-branch
```

Удалить ветку `my-cool-branch`:

```
git branch -D my-cool-branch
```

Создать и перейти:

```
git checkout -B my-cool-branch
```

Просмотреть статус:

```
git status
```

Добавить все файлы:

```
git add *
```

Добавить файл `mycoolfile`:

```
git add mycoolfile
```

Создать коммит с сообщением `test message`:

```
git commit -m "test message"
```

Отправить изменения в удаленный репозиторий

```
git push
```

# work scenario

```shell
git pull
git checkout -B <new-branch>
# some work inside
git add *
git commit -m <message>
git push
```