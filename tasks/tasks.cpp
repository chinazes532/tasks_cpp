// tasks.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "tasks.h"
#include <string>     
#include <vector>  

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Tasks(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    AddTask(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TASKS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASKS));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


class Task {
private:
    int id;
    std::wstring name;
    std::wstring description;
    bool status;

public:
    // Конструктор: статус по умолчанию false
    Task(int id, const std::wstring& name, const std::wstring& description, bool status = false)
        : id(id), name(name), description(description), status(status) {}

    // Геттеры
    int GetId() const { return id; }
    std::wstring GetName() const { return name; }
    std::wstring GetDescription() const { return description; }
    bool GetStatus() const { return status; }

    // Сеттеры
    void SetName(const std::wstring& newName) { name = newName; }
    void SetDescription(const std::wstring& newDesc) { description = newDesc; }
    void SetStatus(bool newStatus) { status = newStatus; }
};

class TaskManager {
private:
    std::vector<Task> tasks;
    int nextId = 1;  // Счётчик для уникальных ID

public:
    // Добавить новую задачу, ID проставится автоматически, статус false по умолчанию
    void AddTask(const std::wstring& name, const std::wstring& description) {
        tasks.emplace_back(nextId++, name, description, false);
    }

    // Получить все задачи (пример)
    const std::vector<Task>& GetTasks() const { return tasks; }

    // Можно добавить методы для удаления, изменения и т.п.
};


// Вектор для хранения задач (глобальный для простоты)
std::vector<TaskManager> tasks;
//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASKS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TASKS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_32771: // кнопка Задачи
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Tasks);
                //DestroyWindow(hWnd);
                break;
            case ID_32772: // кнопка Добавить задачу
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, AddTask);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Определение глобальной переменной (только здесь!)
TaskManager g_taskManager;


INT_PTR CALLBACK Tasks(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG: {
        SetWindowText(hDlg, L"Трекер задач");

        // Получаем дескриптор ListBox (замени IDC_LIST1 на свой ID, если отличается)
        HWND hListBox = GetDlgItem(hDlg, IDC_LIST1);
        if (!hListBox) {
            MessageBox(hDlg, L"ListBox не найден!", L"Ошибка", MB_OK | MB_ICONERROR);
            return (INT_PTR)TRUE;
        }

        // Очищаем список (на случай повторного открытия)
        SendMessage(hListBox, LB_RESETCONTENT, 0, 0);

        // Получаем задачи и добавляем в ListBox
        const auto& tasks = g_taskManager.GetTasks();  // Предполагаем, что есть метод GetTasks()
        for (size_t i = 0; i < tasks.size(); ++i) {
            const Task& task = tasks[i];

            // Формируем строку: "ID: 1 | Имя: TaskName | Описание: Description | Статус: Active"
            std::wstring itemText = L"ID: " + std::to_wstring(task.GetId()) +
                L" | Имя: " + task.GetName() +
                L" | Описание: " + task.GetDescription() +
                L" | Статус: " + (task.GetStatus() ? L"Выполнена" : L"Не выполнена");

            // Добавляем строку в ListBox
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)itemText.c_str());
        }

        // Если задач нет, добавляем сообщение
        if (tasks.empty()) {
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Нет задач.");
        }

        return (INT_PTR)TRUE;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}



// Функции диалогов
INT_PTR CALLBACK AddTask(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        SetWindowText(hDlg, L"Добавление задачи");
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BUTTON1: {
            // Буферы для текста
            wchar_t name[256] = { 0 };
            wchar_t description[1024] = { 0 };

            // Получаем текст из полей
            GetDlgItemText(hDlg, IDC_EDIT1, name, _countof(name));
            GetDlgItemText(hDlg, IDC_EDIT2, description, _countof(description));

            // Проверка на пустое имя
            if (wcslen(name) == 0) {
                MessageBox(hDlg, L"Введите имя задачи.", L"Ошибка", MB_OK | MB_ICONERROR);
                break;
            }

            // Добавляем задачу
            g_taskManager.AddTask(name, description);

            // Очищаем поля и показываем успех
            SetDlgItemText(hDlg, IDC_EDIT1, L"");
            SetDlgItemText(hDlg, IDC_EDIT2, L"");
            MessageBox(hDlg, L"Задача добавлена!", L"Успех", MB_OK | MB_ICONINFORMATION);
            break;
        }
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
} // -_-

