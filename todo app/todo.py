import os
import json
import datetime


current_profile = None


class Todo:
    def __init__(self, name, description, date, is_done=False):
        self.name = name
        self.description = description
        self.date = date
        self.is_done = is_done


class Profile:
    def __init__(self, name):
        self.name = name
        self.todos = []

    def add_todo(self, todo):
        self.todos.append(todo)

    def update_todo(self, index, todo):
        if index < len(self.todos):
            self.todos[index] = todo
        else:
            print("Invalid index.")

    def remove_todo(self, index):
        if index < len(self.todos):
            self.todos.pop(index)
        else:
            print("Invalid index.")

    def check_todo(self, index):
        if index < len(self.todos):
            self.todos[index].is_done = True
        else:
            print("Invalid index.")

    def order_todos_by_date(self):
        self.todos.sort(key=lambda x: x.date)

    def search_todos(self, keyword):
        results = []
        for todo in self.todos:
            if keyword.lower() in todo.name.lower() or keyword.lower() in todo.description.lower():
                results.append(todo)
        return results


class TodoApp:
    def __init__(self):
        self.profiles = []
        self.load_profiles()

    def create_profile(self, name):
        profile = Profile(name)
        self.profiles.append(profile)
        self.save_profiles()

    def switch_profile(self, index):
        global current_profile
        if index < len(self.profiles):
            current_profile = self.profiles[index]
        else:
            print("Invalid index.")

    def remove_profile(self, index):
        if index < len(self.profiles):
            self.profiles.pop(index)
            self.save_profiles()
        else:
            print("Invalid index.")

    def load_profiles(self):
        if os.path.isfile("profiles.json"):
            with open("profiles.json", "r") as f:
                profiles_data = json.load(f)
            for profile_data in profiles_data:
                profile = Profile(profile_data["name"])
                for todo_data in profile_data["todos"]:
                    todo = Todo(
                        todo_data["name"],
                        todo_data["description"],
                        datetime.datetime.strptime(
                            todo_data["date"], "%Y-%m-%d").date(),
                        todo_data["is_done"]
                    )
                    profile.add_todo(todo)
                self.profiles.append(profile)

    def save_profiles(self):
        profiles_data = []
        for profile in self.profiles:
            todos_data = []
            for todo in profile.todos:
                todo_data = {
                    "name": todo.name,
                    "description": todo.description,
                    "date": todo.date.strftime("%Y-%m-%d"),
                    "is_done": todo.is_done
                }
                todos_data.append(todo_data)
            profile_data = {
                "name": profile.name,
                "todos": todos_data
            }
            profiles_data.append(profile_data)

        with open("profiles.json", "w") as f:
            json.dump(profiles_data, f, indent=4)


def clear_terminal():
    os.system('cls' if os.name == 'nt' else 'clear')


def display_menu():
    print("========== TODO APP ==========")
    print("1. Add Todo")
    print("2. Update Todo")
    print("3. Remove Todo")
    print("4. Check Todo")
    print("5. List Todos")
    print("6. Search Todos")
    print("7. Switch Profile")
    print("8. Create Profile")
    print("9. Remove Profile")
    print("10. List Profiles")
    print("0. Quit")


def add_todo():
    name = input("Enter the name of the todo: ")
    description = input("Enter the description of the todo: ")
    date_str = input("Enter the due date (YYYY-MM-DD) of the todo: ")
    try:
        date = datetime.datetime.strptime(date_str, "%Y-%m-%d").date()
        todo = Todo(name, description, date)
        current_profile.add_todo(todo)
        print("Todo added successfully.")
    except ValueError:
        print("Invalid date format.")


def update_todo():
    index = int(input("Enter the index of the todo to update: "))
    name = input("Enter the new name of the todo: ")
    description = input("Enter the new description of the todo: ")
    date_str = input("Enter the new due date (YYYY-MM-DD) of the todo: ")
    try:
        date = datetime.datetime.strptime(date_str, "%Y-%m-%d").date()
        todo = Todo(name, description, date)
        current_profile.update_todo(index, todo)
        print("Todo updated successfully.")
    except ValueError:
        print("Invalid date format.")


def remove_todo():
    index = int(input("Enter the index of the todo to remove: "))
    current_profile.remove_todo(index)
    print("Todo removed successfully.")


def check_todo():
    index = int(input("Enter the index of the todo to check: "))
    current_profile.check_todo(index)
    print("Todo checked successfully.")


def list_todos():
    if len(current_profile.todos) > 0:
        print("========== TODOS ==========")
        for i, todo in enumerate(current_profile.todos):
            status = "[✔️]" if todo.is_done else "[X]"
            print(f"{i}. {status} {todo.name} - {todo.date.strftime('%Y-%m-%d')}")
    else:
        print("No todos found.")


def search_todos():
    keyword = input("Enter the keyword to search in todos: ")
    results = current_profile.search_todos(keyword)
    if len(results) > 0:
        print("========== SEARCH RESULTS ==========")
        for i, todo in enumerate(results):
            status = "[x]" if todo.is_done else "[ ]"
            print(f"{i}. {status} {todo.name} - {todo.date.strftime('%Y-%m-%d')}")
    else:
        print("No results found.")


def switch_profile():
    index = int(input("Enter the index of the profile to switch: "))
    app.switch_profile(index)
    print(f"Switched to profile: {current_profile.name}")


def create_profile():
    name = input("Enter the name of the new profile: ")
    app.create_profile(name)
    print(f"Profile '{name}' created successfully.")


def remove_profile():
    index = int(input("Enter the index of the profile to remove: "))
    app.remove_profile(index)
    print("Profile removed successfully.")


def list_profiles():
    if len(app.profiles) > 0:
        print("========== PROFILES ==========")
        for i, profile in enumerate(app.profiles):
            print(f"{i}. {profile.name}")
    else:
        print("No profiles found.")


app = TodoApp()


if len(app.profiles) > 0:
    current_profile = app.profiles[0]
else:
    app.create_profile("DEFAULT")
    current_profile = app.profiles[0]


while True:
    clear_terminal()
    display_menu()
    choice = input("Enter your choice: ")

    clear_terminal()

    if choice == '1':
        add_todo()
    elif choice == '2':
        update_todo()
    elif choice == '3':
        remove_todo()
    elif choice == '4':
        check_todo()
    elif choice == '5':
        current_profile.order_todos_by_date()
        list_todos()
    elif choice == '6':
        search_todos()
    elif choice == '7':
        switch_profile()
    elif choice == '8':
        create_profile()
    elif choice == '9':
        remove_profile()
    elif choice == '10':
        list_profiles()
    elif choice == '0':
        break
    else:
        print("Invalid choice.")

    input("Press Enter to continue...")
