<div>
  <img src="Syringa.ico" alt="Syringa Logo" align="left" width="130" height="130" style="margin-top: 30px;">
  <h1>💉 Syringa Language</h1>
</div>

_An interpreter designed from the ground up to execute Syringa code. It also serves as an educational resource, promoting learning and encouraging contributions to the open-source Syringa ecosystem._

# 👀 Project Overview

Welcome to Syringa Interpreter in C++ project! This project aims to provide a robust and feature-rich interpreter for the new programming language Syringa, built entirely from scratch in C++. Whether you're looking to explore the internals of how an interpreter works, experiment with language features, this interpreter is a powerful tool designed with flexibility and extensibility in mind.

# 🔑 _Key Features_
  ### 🧠 _Dynamic Typing_
Syringa supports dynamic typing, allowing variables to change types at runtime. This feature provides flexibility, making it easier to write and execute dynamic code.

  ### 🗂️ _Classes and Instances_
Support for Object-Oriented Programming is implemented! You can define your own classes, create instances, and access attributes and methods.

  ### ➕ _Operator Overloading_
The interpreter supports operator overloading, allowing you to define specific behaviors for operators like +, -, *, /, and more, within your custom classes.

  ### ⨐ _Function Overloading_
The interpreter supports function overloading, allowing you to define different behaviors for same function name.

  ### 📜 _Execution System and Parser_
The architecture of the interpreter includes a robust parser and execution system that translates Syringa code into executable operations. This allows for dynamic interpretation and real-time code execution.

  ### ♻️ _Garbage Collector_
To manage memory efficiently, the project includes an integrated garbage collector, based on built-in smart pointer in C++ 'std::shared_ptr', which helps prevent memory leaks by automatically freeing unused objects.

## 💻📚 _Resources_
  + ### _Videos_
    + [Make YOUR OWN Programming Language in Python
](https://youtu.be/Eythq9848Fg?si=fLQSmZx4BOOp2s44) Playlist on YouTube
    + [I made my own programming language, HERE'S HOW!
](https://youtu.be/4Wn54XtgeJM?si=TiR2qOobqEuywqfQ)
  + ### _Open-Source Projects_
    + [Python Interpreter](https://github.com/BRCode4Fun/Python-Interpreter)
    + [cpython](https://github.com/python/cpython)
    + [cython](https://github.com/cython/cython)

## 🐞 _Bugs_
There are some bugs in this project such as:
  + Run-Time Error when creating nested classes or functions.
  + An infinite loop when there is an error during parsing
  + Modules, Classes, Instances.
  + Scopes bugs.
  + Some other bugs.




# 🛠️ Building

```bash
$ git clone https://github.com/Captain3BoOd/Syringa.git
$ cd Syringa
$ make
```

# 🚀 Future steps:
  + Set, Dict, Pointers, Byte Code, etc.
  + Static typing for variables and functions.
  + BigInt class with Karatsuba algorithm.
  + REPL mode.
  + Virtual Machine.
  + Better type checking and error handling.
  + Fixing All bugs.
  + Iterator and generator objects.
  + Inheritance.
  + New error types.
  + etc.

# 🤝 Contributions
This project is under continuous development, with new features being added regularly. If you’re passionate about Syringa, C++, or just interested in contributing to an exciting open-source project, feel free to jump in!

## How to Contribute
 + Fork this repository.
 + Create a new branch for your feature (git checkout -b feature/NewFeature).
 + Commit your changes (git commit -m 'Add NewFeature').
 + Push to the branch (git push origin feature/NewFeature).
 + Open a Pull Request.

Any form of contribution is welcome, whether it’s code, documentation, or testing. Additionally, feel free to share this project with friends, colleagues, or within developer communities.
