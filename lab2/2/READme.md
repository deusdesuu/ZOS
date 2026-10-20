# 2. Необходимо сформировать матрицу вещественных чисел, содержащую n строк и m столбцов, а затем найти ее сумму. Формирование строк матрицы должно выполняться параллельно n потоками. Подсчет суммы элементов также должен выполняться n параллельными потоками. Поток подсчета суммы по строке может быть запущен только тогда, когда поток формирования строки матрицы закончит свою работу. Для повышения надежности синхронизации рекомендуется потоки подсчета сумм запустить в приостановленном состоянии

## Глобальные переменные

<img width="240" height="134" alt="image" src="https://github.com/user-attachments/assets/d51386ee-0dae-483f-9988-94942b23997e" />

## Функция вывода матрицы

<img width="361" height="165" alt="image" src="https://github.com/user-attachments/assets/c6875eb2-7050-48bd-8aac-98e0d363a001" />

## Функция заполнения матрицы

<img width="433" height="142" alt="image" src="https://github.com/user-attachments/assets/52aa6436-0623-42b5-b58f-6f4ddb5ec4cf" />

## Функция суммирования матрицы

<img width="315" height="214" alt="image" src="https://github.com/user-attachments/assets/543505c2-268a-4057-9361-b9a470ef73da" />

## Работа с потоками

- потоки для суммирования создаем с флагом CREATE_SUSPENDED
- запускаем через ResumeThread после заполнения матрицы

<img width="823" height="464" alt="image" src="https://github.com/user-attachments/assets/df0ee7ab-a5d3-4e29-8f48-f7326a609458" />

## Результат работы

<img width="720" height="242" alt="image" src="https://github.com/user-attachments/assets/c680d8e5-b0f2-427f-9bec-59114f8f9274" />
