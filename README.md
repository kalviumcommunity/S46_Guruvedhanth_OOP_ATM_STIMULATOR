# ATM Simulation Project

## Overview

This project is a simple ATM simulation implemented in C++. It demonstrates the core concepts of Object-Oriented Programming (OOP), specifically focusing on **classes** and **objects**. The simulation includes two main classes: `Account` and `ATM`. The `Account` class represents a user's bank account, while the `ATM` class simulates an ATM machine interacting with the bank account.

## Project Structure

- **Account Class**: 
  - Represents a bank account with an account number and balance.
  - Contains member functions to perform deposits and withdrawals.
  
- **ATM Class**: 
  - Represents an ATM machine.
  - Contains member functions to interact with the `Account` class for depositing and withdrawing money.

## Features

- **Deposit Money**: The ATM can deposit money into the linked account.
- **Withdraw Money**: The ATM can withdraw money from the linked account, provided there are sufficient funds.
- **Balance Check**: The account balance is checked during withdrawals to prevent overdrafts.