# WatchCat

## Проект watchcat

Проект на с++ использует вебкамеру

подключить opencv

vsbaranov83@gmail.com Владимир Баранов

Дата создания: 13.03.21 14:52

## Основные фнкции 
1. запись видео при срабатывании триггера
   аудио триггер (порог громкости превышает заданное значение)
   видео триггер (в кадре есть движение)
   речевой триггер( есть определенные слова или сочетания слов в речи )
   использовать пайп в ffmpeg

2. Передача емайла при срабатывании тригера (посмотреть скрипт на пайтоне)

3. Включение сирены (сирена сначала будет мой голос запитченный на пару тонов для устрашения)

4. Идентификация по лицу для отмены тревоги. Лица членов семьи.

5. Уничтожение заголовка раздела 



!! Программа долна работать в фоновом режиме. Поэтому традиционный qt GUI вариант делать не надо.
Не обязательно. Если иксы не сбрасывать а просто блокировать компьютер.
Для интерфейса и управления можно использовать библиотеку highgui.

TODO: Найти программы по идентификации по лицу

## Обработка аудио

Библиотека kfr оптимизирована для работы в реальном времени.
https://github.com/kfrlib/kfr/blob/master/examples/dft.cpp

## Обработка видео
Библиотека opencv 
Дополнительные библиотеки для motion tracking и идентификации по лицу.


## Hardware
В качестве платформы можно использовать rasbery pi с дебианом.
Можно использовать андройд смартфон в качестве мозгового центра.
Можно помотреть какие продаются датчики движения.


## Алгоритм детекции движения:

1. Берется два последовательных кадра.
2. Берется серые компоненты
3. Гаусс сглаживания обоих кадров
4. Абсолютная разница между кадрами
5. Морфологический фильтр dilate
6. Поиск контуров
7. Нахождение максимального контура по площади
8. Срабатывание тревоги если таких контуров больше 3.


