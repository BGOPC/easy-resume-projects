const readline = require('readline');
const fs = require('fs');

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

class Todo {
  constructor(name, description, date, is_done = false) {
    this.name = name;
    this.description = description;
    this.date = date;
    this.is_done = is_done;
  }
}

class Profile {
  constructor(name) {
    this.name = name;
    this.todos = [];
  }

  addTodo(todo) {
    this.todos.push(todo);
  }

  updateTodo(index, todo) {
    if (index < this.todos.length) {
      this.todos[index] = todo;
    } else {
      console.log("Invalid index.");
    }
  }

  removeTodo(index) {
    if (index < this.todos.length) {
      this.todos.splice(index, 1);
    } else {
      console.log("Invalid index.");
    }
  }

  checkTodo(index) {
    if (index < this.todos.length) {
      this.todos[index].is_done = true;
    } else {
      console.log("Invalid index.");
    }
  }

  orderTodosByDate() {
    this.todos.sort((a, b) => new Date(a.date) - new Date(b.date));
  }

  searchTodos(keyword) {
    const results = [];
    for (const todo of this.todos) {
      if (todo.name.toLowerCase().includes(keyword.toLowerCase()) || todo.description.toLowerCase().includes(keyword.toLowerCase())) {
        results.push(todo);
      }
    }
    return results;
  }
}

class TodoApp {
  constructor() {
    this.profiles = [];
    this.loadProfiles();
  }

  createProfile(name) {
    const profile = new Profile(name);
    this.profiles.push(profile);
    this.saveProfiles();
  }

  switchProfile(name) {
    let index = this.profiles.indexOf(name);
    if (index < this.profiles.length) {
      current_profile = this.profiles[index];
    } else {
      console.log("Invalid index.");
    }
  }

  removeProfile(name) {
    let index = this.profiles.indexOf(name);
    if (index < this.profiles.length) {
      this.profiles.splice(index, 1);
      this.saveProfiles();
    } else {
      console.log("Invalid index.");
    }
  }

  loadProfiles() {
    if (fs.existsSync("profiles.json")) {
      const data = fs.readFileSync("profiles.json", "utf8");
      const profilesData = JSON.parse(data);
      for (const profileData of profilesData) {
        const profile = new Profile(profileData.name);
        for (const todoData of profileData.todos) {
          const todo = new Todo(
            todoData.name,
            todoData.description,
            new Date(todoData.date),
            todoData.is_done
          );
          profile.addTodo(todo);
        }
        this.profiles.push(profile);
      }
    }
  }

  saveProfiles() {
    const profilesData = this.profiles.map((profile) => {
      const todosData = profile.todos.map((todo) => ({
        name: todo.name,
        description: todo.description,
        date: todo.date.toISOString().split('T')[0],
        is_done: todo.is_done
      }));
      return {
        name: profile.name,
        todos: todosData
      };
    });

    fs.writeFileSync("profiles.json", JSON.stringify(profilesData, null, 4));
  }
}

function clearTerminal() {
  console.clear();
}

function displayMenu() {
  console.log("========== TODO APP ==========");
  console.log("1. Add Todo");
  console.log("2. Update Todo");
  console.log("3. Remove Todo");
  console.log("4. Check Todo");
  console.log("5. List Todos");
  console.log("6. Search Todos");
  console.log("7. Switch Profile");
  console.log("8. Create Profile");
  console.log("9. Remove Profile");
  console.log("10. List Profiles");
  console.log("0. Quit");
}

const app = new TodoApp();
let current_profile = null;

if (app.profiles.length > 0) {
  current_profile = app.profiles[0];
} else {
  app.createProfile("DEFAULT");
  current_profile = app.profiles[0];
}

function addTodo() {
  rl.question("Enter the name of the todo: ", (name) => {
    rl.question("Enter the description of the todo: ", (description) => {
      rl.question("Enter the due date (YYYY-MM-DD) of the todo: ", (dateStr) => {
        const date = new Date(dateStr);
        if (isNaN(date)) {
          console.log("Invalid date format.");
        } else {
          const todo = new Todo(name, description, date);
          current_profile.addTodo(todo);
          console.log("Todo added successfully.");
        }
        rl.prompt();
      });
    });
  });
}

function updateTodo() {
  rl.question("Enter the index of the todo to update: ", (indexStr) => {
    const index = parseInt(indexStr);
    if (index >= 0 && index < current_profile.todos.length) {
      rl.question("Enter the new name of the todo: ", (name) => {
        rl.question("Enter the new description of the todo: ", (description) => {
          rl.question("Enter the new due date (YYYY-MM-DD) of the todo: ", (dateStr) => {
            const date = new Date(dateStr);
            if (isNaN(date)) {
              console.log("Invalid date format.");
            } else {
              const todo = new Todo(name, description, date);
              current_profile.updateTodo(index, todo);
              console.log("Todo updated successfully.");
            }
            rl.prompt();
          });
        });
      });
    } else {
      console.log("Invalid index.");
      rl.prompt();
    }
  });
}

function removeTodo() {
  rl.question("Enter the index of the todo to remove: ", (indexStr) => {
    const index = parseInt(indexStr);
    if (index >= 0 && index < current_profile.todos.length) {
      current_profile.removeTodo(index);
      console.log("Todo removed successfully.");
    } else {
      console.log("Invalid index.");
    }
    rl.prompt();
  });
}

function checkTodo() {
  rl.question("Enter the index of the todo to check: ", (indexStr) => {
    const index = parseInt(indexStr);
    if (index >= 0 && index < current_profile.todos.length) {
      current_profile.checkTodo(index);
      console.log("Todo checked successfully.");
    } else {
      console.log("Invalid index.");
    }
    rl.prompt();
  });
}

function listTodos() {
  if (current_profile.todos.length > 0) {
    console.log("========== TODOS ==========");
    current_profile.orderTodosByDate();
    current_profile.todos.forEach((todo, index) => {
      const status = todo.is_done ? "[✔️]" : "[X]";
      console.log(`${index}. ${status} ${todo.name} - ${todo.date.toISOString().split('T')[0]}`);
    });
  } else {
    console.log("No todos found.");
  }
  rl.prompt();
}

function searchTodos() {
  rl.question("Enter the keyword to search in todos: ", (keyword) => {
    const results = current_profile.searchTodos(keyword);
    if (results.length > 0) {
      console.log("========== SEARCH RESULTS ==========");
      results.forEach((todo, index) => {
        const status = todo.is_done ? "[x]" : "[ ]";
        console.log(`${index}. ${status} ${todo.name} - ${todo.date.toISOString().split('T')[0]}`);
      });
    } else {
      console.log("No results found.");
    }
    rl.prompt();
  });
}

function switchProfile() {
  rl.question("Enter the name of the profile to switch: ", (name) => {
    app.switchProfile(name);
    if (current_profile) {
      console.log(`Switched to profile: ${current_profile.name}`);
    } else {
      console.log("Invalid index.");
    }
    rl.prompt();
  });
}

function createProfile() {
  rl.question("Enter the name of the new profile: ", (name) => {
    app.createProfile(name);
    console.log(`Profile '${name}' created successfully.`);
    rl.prompt();
  });
}

function removeProfile() {
  rl.question("Enter the index of the profile to remove: ", (indexStr) => {
    const index = parseInt(indexStr);
    app.removeProfile(index);
    console.log("Profile removed successfully.");
    rl.prompt();
  });
}

function listProfiles() {
  if (app.profiles.length > 0) {
    console.log("========== PROFILES ==========");
    app.profiles.forEach((profile, index) => {
      console.log(`${index}. ${profile.name}`);
    });
  } else {
    console.log("No profiles found.");
  }
  rl.prompt();
}

rl.on('line', (input) => {
  clearTerminal();
  displayMenu();

  switch (input) {
    case '1':
      addTodo();
      break;
    case '2':
      updateTodo();
      break;
    case '3':
      removeTodo();
      break;
    case '4':
     checkTodo();
      break;
    case '5':
      listTodos();
      break;
    case '6':
      searchTodos();
      break;
    case '7':
      switchProfile();
      break;
    case '8':
      createProfile();
      break;
    case '9':
      removeProfile();
      break;
    case '10':
      listProfiles();
      break;
    case '0':
      rl.close();
      break;
    default:
      console.log("Invalid choice.");
      rl.prompt();
      break;
  }
}).on('close', () => {
  console.log("Quitting...");
  process.exit(0);
});

rl.prompt();