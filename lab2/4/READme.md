# 4. Написать программу формирования дека из элементов файла с помощью n потоков. Каждый поток может добавить элемент в начало или конец дека. Решение о месте вставки принимается случайным образом. Если в это время другой поток модифицирует выбранный участок дека, поток должен перейти в состояние ожидания.

## Идея
- считаем файл в буфер
- будем записывать элементы в 2 разных дека:
  - dq_front, если нарандомило push_front
  - dq_back, если push_back
- в конце сделаем конкатенацию

## Глобальные переменные

<img width="263" height="142" alt="image" src="https://github.com/user-attachments/assets/daeb1bff-f95f-4a15-97a6-c46708d839f9" />

## Функция вывода

<img width="385" height="112" alt="image" src="https://github.com/user-attachments/assets/1703fde6-b3eb-4232-8fc5-f46ef98c073e" />

## Функция заполнения дека (деков)

- считываем элементы в 2 разных дека
- при этом у каждого есть своя CRITICAL_SECTION
- "Если в это время другой поток модифицирует выбранный участок дека, поток должен перейти в состояние ожидания." соблюдается наличием CRITICAL_SECTION

<img width="478" height="428" alt="image" src="https://github.com/user-attachments/assets/84694d22-fe97-46b6-b731-3efb2c225a29" />

## Работа с потоками, конкатенация, вывод результата

<img width="809" height="610" alt="image" src="https://github.com/user-attachments/assets/994b52a0-04d8-44f6-a994-6c65678e097f" />

## Результат работы программы (10 элементов 4 потока)

<img width="671" height="190" alt="image" src="https://github.com/user-attachments/assets/9cadcd4f-db95-4d43-9153-fd39d3cb646b" />

## Результат работы программы (10_000 элементов 10 потоков)(не ломается)

<img width="1540" height="853" alt="image" src="https://github.com/user-attachments/assets/37a9ac12-d8b5-46a5-ad7b-8fb405b23d68" />
