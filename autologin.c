int main() 
{
	execlp( "login", "login", "-f", "timerctrl", 0);
        execlp( "/usr/sbin/time", 0,0,0,0);

}

