I developed this project in the early 2000s.

It is a time control system (presence) for the company.

It was a client/server architecture. 
A mini terminal with a keyboard, an LCD display and a card reader, manufactured by the company, which was connected to a Linux server via RS232.
A Linux Server
A Desktop Manager application (Windows)

Through the terminal, each operator introduced a card both at the beginning of the working day and at the end of work.

The data was recorded in a MySQL database and each card was associated with a user profile in an OpenLDAP database.

All this data was managed through a Windows C++ (MFC) app.

The terminal firmware is written in C for the microprocessor: MC68HC705C8/A
