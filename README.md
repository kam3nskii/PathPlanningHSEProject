# PathPlanningProject
## Требования
Для сборки и запуска возможно использовать CMake. 
CMakeLists.txt и .pro файлы доступны в репозитории. 
Для проведения тестирования локально используйте CMake. Подробные требования к ПО указаны ниже. 

### Linux
- Git 2.7.4 или выше
- CMake 3.2 или выше
- GCC 4.9 или выше
- Make

### Mac
- Git 2.23.0 или выше
- CMake 3.2 или выше
- Apple LLVM version 10.0.0 (clang-1000.11.45.5) или выше
- Make

### Windows
- Git 2.23.0 или выше
- CMake 3.2 или выше
- MinGW-w64 5.0.3 или выше (должен быть добавлен в переменную среды Path)

## Начало работы
### Сборка и запуск

При использовании CMake сборка и запуск может производиться как из командной строки, так и при помощи различных IDE (например JetBrains CLion). Ниже приведены скрипты сборки и запуска с использованием командной строки.

#### Linux и Mac
Release сборка:
```bash
cd PathPlanningProject
cd Build
cd Release
cmake ../../ -DCMAKE_BUILD_TYPE="Release"
make
make install
```

Debug сборка:
```bash
cd PathPlanningProject
cd Build
cd Debug
cmake ../../ -DCMAKE_BUILD_TYPE="Debug"
make
make install
```

Запуск:
```bash
cd ../../Bin/{Debug|Release}/
./PathPlanning ../../Examples/example.xml
```
#### Windows
Release сборка:
```cmd
cd PathPlanningProject
cd Build
cd Release
set PATH
cmake ../../ -DCMAKE_BUILD_TYPE="Release" -G "MinGW Makefiles"
mingw32-make
mingw32-make install
```

Debug сборка:
```cmd
cd PathPlanningProject
cd Build
cd Debug
set PATH
cmake ../../ -DCMAKE_BUILD_TYPE="Debug" -G "MinGW Makefiles"
mingw32-make
mingw32-make install
```

Запуск:
```cmd
cd ../../Bin/{Debug|Release}/
PathPlanning.exe ../../Examples/example.xml
```

## Тестирование 
При использовании сборки CMake возможен запуск тестов.
Локальный запуск тестов производится из директории `.../PathPlanningHSEProject/Build/{Debug|Release}/` с помощью команды:

```
 ctest
```

либо (для более подробного вывода):
```
 ctest --output-on-failure
```

## Контакты
**Каменский Денис**
- dekamenskiy@edu.hse.ru
- Telegram: @kam3nskii
