vuser_end()
{
	if (isNVRunning() > 0) 
		StopNV();
	
	return 0;
}
