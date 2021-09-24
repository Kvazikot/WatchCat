# WatchCat

## Проект watchcat
OS: Linux
OS: Windows (experimental mode)
[скачать Watchcat_Release_1.05_win64.zip](https://github.com/Kvazikot/WatchCat/tags)

Проект на с++ использует вебкамеру
Для сборки нужен opencv и cmake.

Другие программы которые использует watchcat: ffplay и ffmpeg

python нужен чтобы запустить внешний скрипт send_email

подключить opencv

vsbaranov83@gmail.com Владимир Баранов

Дата создания: 13.03.21 14:52

## Основные функции 
1. запись видео при срабатывании триггера
   аудио триггер (порог громкости превышает заданное значение)
   видео триггер (в кадре есть движение)
   речевой триггер( есть определенные слова или сочетания слов в речи )
   использовать пайп в ffmpeg

2. Передача емайла при срабатывании тригера (посмотреть скрипт на пайтоне)

3. Включение сирены (сирена сначала будет мой голос запитченный на пару тонов для устрашения)

4. Идентификация по лицу для отмены тревоги. Лица членов семьи.

5. Уничтожение заголовка раздела 

# Как настроить почту?
Чтобы работала функция отправки почты необходимо: 
1. Установить интерпретатор python3 с официального сайта.
2. Найти провайдера который предоставляет бесплатный smtp-server. Нппример mailjet.
3. Отредактировать файл send_mail.py
В строчке login нужно прописать те значения которые дает сервер при регистрации ящика.
server.login('24ed635ed6e3e35437ce2e33415ea422', "16748719e3322b46a173b5e6791e7f85")
В этом же фале вы можете отредактировать текст сообщения
Редактируйте поля FROM, TO, SUBJECT, TEXT.

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

# Программа watchcat

Программа watchcat нужна, если вы хотите узнать кто входил в вашу комнату пока вас не было.
Возможно вы хотите знать кто садился за кмпьютер.
Все что необходимо для ее работы это компьютер с linux и веб камера.
Если у вас современный ноутбук с камерой, то веб камера вам не нужна.
При появлении значимого движения в кадре, программы включает сирену
и начинает высылать предупреждения на email.
Звуковой файл сирены можно определить опцией -f
Можно записывать видео на диск или пробрасывать его на stdout.
 
```
Usage: WatchCat [params]  
       -d, --show_date
               print date
       -e, --email (value:0)
               email me
       -f, --sound_file (value:0)
               sound file
       -h, --help (value:true)
               print help message
       -i, --input (value:0)
               take input video or stream
       --no_sound, --ns (value:0)
               don't play sound
       --no_video, --nv (value:0)
               don't show video
       -o, --output
               save output video
       -s, --stdout (value:0)
               std output in jmpeg
```
