<div align="center">

![Banner](IMAGES/Asset_manager.png)

# ASSET MANAGER - Keep an eye on your moves

<img src="IMAGES/Asset_manager1.jpg" alt="Portfolio Overview" width="70%"/>

</div>

<div align="center">

## Getting Started

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
Option 1: Command Line Mode
./asset_manager

Option 2: Window Mode
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

## User Guide

</div>

<div style="display: flex; align-items: flex-start;">
  <div style="flex: 1;">
    <img src="IMAGES/Asset_manager2.png" alt="Asset Operations" width="100%"/>
  </div>
  <div style="flex: 1; padding-left: 20px;">
    <h3>Key Operations</h3>

    1. **Portfolio Management**
       - Create/Delete portfolios
       - Add/Withdraw funds
       - Track balance

    2. **Asset Operations**
       - Buy/Sell assets
       - Update prices
       - View transaction history
       - Calculate profits/losses
       - Simulate sales

    3. **Data Analysis**
       - Performance tracking
       - Portfolio distribution
       - Historical data
       - PNL calculations
  </div>
</div>

## Technical Details
- Written in C
- Python/Dash for visualization
- Make-based compilation
- Terminal-based UI
- Real-time data processing

## License
[MIT License](LICENSE)

## Author
Developed by Nesplee (2024)
