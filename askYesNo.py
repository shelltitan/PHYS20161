# askYesNo
# program asks for a yes or no answer

response = input("Is programming awesome?\n");

    while response.lower() != "yes" and response.lower() != "no":
    response = input("Please just say yes or no\n");
    
print("You said " + response);

