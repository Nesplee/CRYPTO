<div align="center">

![Banner](IMAGES/Asset_manager.png)

# ASSET MANAGER - Keep an eye on your moves

During my first month at 42 School, I developed this program as preparation for my initial exam. It allowed me to strengthen core programming concepts learned during the curriculum, including memory management, complex data structures, and header file organization.

<img src="IMAGES/Asset_manager1.jpg" alt="Portfolio Overview" width="70%"/>

</div>

<div align="center">

## Getting Started

Asset Manager offers a complete solution for tracking your investments. Monitor your portfolio in real-time, manage transactions, analyze performance metrics, and visualize your data through an interactive dashboard. Whether you're tracking simple trades or managing complex asset distributions, this tool provides all the essential features for effective portfolio management.

</div>

### Clone & Setup
```bash
# Clone repository
git clone git@github.com:Nesplee/CRYPTO.git
cd CRYPTO

# Install required packages
sudo apt-get update
sudo apt-get install gcc make python3-pip
pip3 install dash pandas plotly
```
### Build & Launch
```bash
# Compile the program
cd CRYPTO.V3
make

# Run the Program
# Option 1: Command Line Mode
./asset_manager

# Option 2: Window Mode
./launcher
```
<div align="center">

## Core Features & Program Interface

</div>

<div align="center">
<img src="IMAGES/Asset_manager3.png" alt="Terminal Interface" width="70%"/>
</div>

### Terminal Management

- Multi-portfolio management
- Transaction history logging
- Intuitive menu navigation
- Colored information display
- Complete portfolio control

<div align="center">
<img src="IMAGES/Asset_manager4.png" alt="Asset Operations" width="70%"/>
</div>

### Asset Operations & Analysis

- Buy/Sell assets management
- Performance tracking
- Profit/Loss calculations
- Transaction simulations
- Portfolio distribution view

<div align="center">

## Interactive Dashboard

<img src="IMAGES/Asset_manager6.png" alt="Dashboard View" width="100%"/>

</div>

- Real-time visualization
- Performance graphs
- Portfolio overview

**Important Note:**
- Dashboard requires a `MAIN.txt` file in the working directory
- Launch with: `python app.py`
- Access via browser at: `http://localhost:8050`
- File format must be JSON with portfolio data

<div align="center">

# User Guide

| ***Portfolio Management*** | ***Asset Operations*** | ***Data Analysis*** |
|:-------------------:|:----------------:|:-------------:|
| - Create/Delete portfolios | - Buy/Sell assets | - Performance tracking |
| - Add/Withdraw funds | - Update prices | - Portfolio distribution |
| - Track balance | - Calculate profits/losses | - Historical data |
| - View transactions | - Simulate sales | - PNL calculations |

<img src="IMAGES/Asset_manager2.png" alt="Asset Operations" width="70%"/>

</div>

<div align="center">

## Technical Details

</div>

- 🔧 Written in C
- 🐍 Python/Dash for visualization
- 📦 Make-based compilation
- 💻 Terminal-based UI
- ⚡ Real-time data processing

<div align="center">
<h3>💻 Tech Stack 💻</h3>

<div align="center" style="display: flex; gap: 10px; justify-content: center;">
<a href="https://en.cppreference.com/w/c/language"><img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/c/c-original.svg" width="45" height="45"/></a>
<a href="https://www.python.org/"><img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/python/python-original.svg" width="45" height="45"/></a>
<a href="https://www.w3.org/html/"><img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/html5/html5-original.svg" width="45" height="45"/></a>
<a href="https://www.w3.org/css/"><img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/css3/css3-original.svg" width="45" height="45"/></a>
<a href="https://code.visualstudio.com/"><img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/vscode/vscode-original.svg" width="45" height="45"/></a>
<a href="https://www.linux.org/"><img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/linux/linux-original.svg" width="45" height="45"/></a>
</div>
</div>

<div align="center">

## 📜 License & Author

<sub>Developed with ❤️ by <a href="https://github.com/Nesplee">Nesplee</a></sub>

<sub>Copyright © 2024 | <a href="LICENSE">MIT License</a></sub>

</div>
