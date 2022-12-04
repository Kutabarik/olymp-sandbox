# Измерение времени исполнения алгоритма в потоке

```c++
_int64 FileTimeToQuadWord(PFILETIME pft)
{
    return(Int64ShllMod32(pft->dwHighDateTime, 32) | pft->dwLowDateTime);
}

void PerformLongOperation ()
{
    FILETIME ftKernelTimeStart, ftKernelTimeEnd;
    FILETIME ftUserTimeStart, ftUserTimeEnd;
    FILETIME ftDummy;

    _int64 qwKernelTimeElapsed, qwUserTimeElapsed, qwTotalTimeElapsed;

    // получаем начальные показатели времени
    GetThreadTimes(GetCurrentThread(), &ftDummy, &ftDummy,
                   &ftKernelTimeStart, &ftUserTimeStart);

    // здесь выполняем сложный алгоритм

    // получаем конечные показатели времени
    GetThreadTimes(GetCurrentThread(), &ftDummy, &ftDummy,
                   &ftKernelTimeEnd, &ftUserTimeEnd);

    // получаем значении времени, затраченного на выполнение ядра и User,
    // преобразуя начальные и конечные показатели времени из FILETIME
    // в учетверенные слова, а затем вычитая начальные показатели из конечных
    qwKernelTimeElapsed = FileTimeToQuadWord(&ftKernelTimeEnd) -
                          FileTimeToQuadWord(&ftKernelTimeStart);

    qwUserTimeElapsed = FileTimeToQuadWord(&ftUserTimeEnd) -
                        FileTimeToQuadWord(&ftUserTimeStart);

    // получаем общее время, складывая время выполнения ядра и User
    qwTotalTimeElapsed = qwKernelTimeElapsed + qwUserTimeElapsed;

    // общее время хранится в qwTotalTimeElapsed

}
```

**qwTotalTimeElapsed** - время, выраженное в интервалах по 100 нс.
