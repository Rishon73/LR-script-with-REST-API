function getProducts(stringData)
{
	//alert(stringData)
	var data = JSON.parse(stringData);
	alert(data.profiles[1].id);

	
	//var value = data.profiles.name; // Specific to the response
	return data.profiles[1].id;
} 