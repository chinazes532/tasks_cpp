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

    // Новый метод для отметки задачи как выполненной
    void SetCompleted(bool completed) { status = completed; }
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

    bool CompleteTask(int id) {
        for (auto& task : tasks) {
            if (task.GetId() == id) {
                task.SetStatus(true);
                return true; // Успешно
            }
        }
        return false; // Задача с таким ID не найдена
    }

    // Получить все задачи (пример)
    const std::vector<Task>& GetTasks() const { return tasks; }

    // Удалить задачу по ID
    bool DeleteTask(int id) {
        auto it = std::remove_if(tasks.begin(), tasks.end(),
            [id](const Task& task) { return task.GetId() == id; });
        if (it != tasks.end()) {
            tasks.erase(it, tasks.end());
            return true; // Успешно
        }
        return false; // Задача с таким ID не найдена
    }
};


// Вектор для хранения задач (глобальный для простоты)
std::vector<TaskManager> tasks;

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


static void RefreshTaskList(HWND hDlg) {
    HWND hListBox = GetDlgItem(hDlg, IDC_LIST1);
    if (!hListBox) return;

    SendMessage(hListBox, LB_RESETCONTENT, 0, 0);

    const auto& tasks = g_taskManager.GetTasks();

    if (tasks.empty()) {
        // Добавляем заглушку и ставим специальное значение item data, чтобы отличать от реальных задач
        LRESULT idx = SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Нет задач.");
        if (idx != LB_ERR && idx != LB_ERRSPACE) {
            SendMessage(hListBox, LB_SETITEMDATA, (WPARAM)idx, (LPARAM)-1);
        }
    }
    else {
        for (size_t i = 0; i < tasks.size(); ++i) {
            const Task& task = tasks[i];
            std::wstring itemText = L"ID: " + std::to_wstring(task.GetId()) +
                L" | Имя: " + task.GetName() +
                L" | Описание: " + task.GetDescription() +
                L" | Статус: " + (task.GetStatus() ? L"Выполнена" : L"Не выполнена");

            LRESULT idx = SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)itemText.c_str());
            if (idx != LB_ERR && idx != LB_ERRSPACE) {
                // Привяжем ID задачи к элементу списка
                SendMessage(hListBox, LB_SETITEMDATA, (WPARAM)idx, (LPARAM)task.GetId());
            }
        }
    }
}

static void UpdateButtonsState(HWND hDlg) {
    HWND hListBox = GetDlgItem(hDlg, IDC_LIST1);
    HWND hBtnComplete = GetDlgItem(hDlg, IDC_BTN_COMPLETE);
    HWND hBtnDelete = GetDlgItem(hDlg, IDC_BTN_DELETE);

    if (!hListBox || !hBtnComplete || !hBtnDelete) return;

    BOOL enable = FALSE;
    LRESULT sel = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
    if (sel != LB_ERR) {
        LRESULT data = SendMessage(hListBox, LB_GETITEMDATA, (WPARAM)sel, 0);
        if (data != LB_ERR && data != (LRESULT)-1) {
            enable = TRUE;
        }
    }
    EnableWindow(hBtnComplete, enable);
    EnableWindow(hBtnDelete, enable);
}

INT_PTR CALLBACK Tasks(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG: {
        SetWindowText(hDlg, L"Трекер задач");

        HWND hListBox = GetDlgItem(hDlg, IDC_LIST1);
        if (!hListBox) {
            MessageBox(hDlg, L"ListBox не найден!", L"Ошибка", MB_OK | MB_ICONERROR);
            return (INT_PTR)TRUE;
        }

        // Создаем кнопки "Выполнить" и "Удалить" рядом с ListBox (если их нет в ресурсах)
        // Пытаемся расположить их под ListBox.
        RECT rcList{}, rcDlg{};
        GetClientRect(hDlg, &rcDlg);
        GetWindowRect(hListBox, &rcList);
        POINT tl{ rcList.left, rcList.top }, br{ rcList.right, rcList.bottom };
        MapWindowPoints(HWND_DESKTOP, hDlg, &tl, 1);
        MapWindowPoints(HWND_DESKTOP, hDlg, &br, 1);
        int listLeft = tl.x;
        int listTop = tl.y;
        int listRight = br.x;
        int listBottom = br.y;
        int btnWidth = 120;
        int btnHeight = 26;
        int margin = 8;

        HWND hBtnComplete = CreateWindowExW(
            0, L"BUTTON", L"Выполнить",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            listLeft, listBottom + margin, btnWidth, btnHeight,
            hDlg, (HMENU)IDC_BTN_COMPLETE, (HINSTANCE)GetWindowLongPtr(hDlg, GWLP_HINSTANCE), nullptr
        );

        HWND hBtnDelete = CreateWindowExW(
            0, L"BUTTON", L"Удалить",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            listLeft + btnWidth + margin, listBottom + margin, btnWidth, btnHeight,
            hDlg, (HMENU)IDC_BTN_DELETE, (HINSTANCE)GetWindowLongPtr(hDlg, GWLP_HINSTANCE), nullptr
        );

        (void)hBtnComplete;
        (void)hBtnDelete;

        // Заполняем список
        RefreshTaskList(hDlg);
        UpdateButtonsState(hDlg);

        return (INT_PTR)TRUE;
    }

    case WM_COMMAND: {
        const WORD id = LOWORD(wParam);
        const WORD code = HIWORD(wParam);

        if (id == IDC_LIST1 && code == LBN_SELCHANGE) {
            UpdateButtonsState(hDlg);
            return (INT_PTR)TRUE;
        }

        if (id == IDC_BTN_COMPLETE && code == BN_CLICKED) {
            HWND hListBox = GetDlgItem(hDlg, IDC_LIST1);
            LRESULT sel = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
            if (sel == LB_ERR) {
                MessageBox(hDlg, L"Выберите задачу.", L"Внимание", MB_OK | MB_ICONINFORMATION);
                return (INT_PTR)TRUE;
            }
            LRESULT taskId = SendMessage(hListBox, LB_GETITEMDATA, (WPARAM)sel, 0);
            if (taskId == (LRESULT)-1 || taskId == LB_ERR) {
                MessageBox(hDlg, L"Эта строка не является задачей.", L"Внимание", MB_OK | MB_ICONWARNING);
                return (INT_PTR)TRUE;
            }

            // Вызовите ваш метод завершения задачи по ID
            // Пример: g_taskManager.CompleteTask((int)taskId);
            if (!g_taskManager.CompleteTask((int)taskId)) {
                MessageBox(hDlg, L"Не удалось выполнить задачу.", L"Ошибка", MB_OK | MB_ICONERROR);
            }

            RefreshTaskList(hDlg);
            UpdateButtonsState(hDlg);
            return (INT_PTR)TRUE;
        }

        if (id == IDC_BTN_DELETE && code == BN_CLICKED) {
            HWND hListBox = GetDlgItem(hDlg, IDC_LIST1);
            LRESULT sel = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
            if (sel == LB_ERR) {
                MessageBox(hDlg, L"Выберите задачу.", L"Внимание", MB_OK | MB_ICONINFORMATION);
                return (INT_PTR)TRUE;
            }
            LRESULT taskId = SendMessage(hListBox, LB_GETITEMDATA, (WPARAM)sel, 0);
            if (taskId == (LRESULT)-1 || taskId == LB_ERR) {
                MessageBox(hDlg, L"Эта строка не является задачей.", L"Внимание", MB_OK | MB_ICONWARNING);
                return (INT_PTR)TRUE;
            }

            if (IDYES != MessageBox(hDlg, L"Удалить выбранную задачу?", L"Подтверждение", MB_YESNO | MB_ICONQUESTION)) {
                return (INT_PTR)TRUE;
            }

            // Вызовите ваш метод удаления задачи по ID
            // Пример: g_taskManager.DeleteTask((int)taskId);
            if (!g_taskManager.DeleteTask((int)taskId)) {
                MessageBox(hDlg, L"Не удалось удалить задачу.", L"Ошибка", MB_OK | MB_ICONERROR);
            }

            RefreshTaskList(hDlg);
            UpdateButtonsState(hDlg);
            return (INT_PTR)TRUE;
        }

        if (id == IDOK || id == IDCANCEL) {
            EndDialog(hDlg, id);
            return (INT_PTR)TRUE;
        }
        break;
    }
    }
    return (INT_PTR)FALSE;
}



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
}