#Write a program that reads in GBplaces.csv and write the data out to a new file in the same format,
#but with the places ordered by increasing population size. (Assigment week 4)
def openfile(string, mode):
    #  checks if file can be opened
    #  returns true if it can be
    # takes two string as argument

    try:

        file = open(string, mode)

        file.close()

        return True

    except:

        print("File " + string + " cannot be opened!")

        return False

def isnumber(arg):
    # function used to check the input whether it is convertible to float or not

    try:

        float(arg)

        return True

    except:

        return False


def sortingpopulation(file):
    # takes a reference to a file with text
    # sorts the lines in ascending order according to the second column if the column contains numbers
    # returns the sorted array if there is an error it returns an empty array

    lines = []  # array made to contain lines

    for line in file:

        linearray = line.split(",")  # splitting the line

        lines.append(linearray) # we add the line to out array
    for i in lines:
        if(i[0] == "% place"):
            lines.pop(lines.index(i))  # we remove the first element it contains no data

    for x in lines:  # checking each line if it contains numbers in the population

        if not isnumber(x[2]):

            print("The population contains non integer elements.")

            print(x)

            return []

    return sorted(lines, key=lambda x: float(x[2]))  # we use the sorting function and we give the lines as input
    #  then we give a function(lambda) that converts every second element of the line to a float we could convert to
    # integer too if we want


def main():
    # main function
    # calls a function to check if the file can be accessed or not

    if not openfile("GBplaces.csv", "r"):  # checking if GBplaces.csv can be opened if not return 0

        return 0

    file = open("GBplaces.csv", "r")  # opening GBplaces.csv

    linessorted = sortingpopulation(file)  # sorting the lines inside the file and saving them in array

    if linessorted == []:  # if the array is empty we say that and return 0

        print("The population could not be sorted.")

        return 0

    if not openfile("GBplacesSorted.csv", "w+"):  # checking if GBplacesSorted.csv can be opened if not return 0

        return 0

    filesorted = open("GBplacesSorted.csv", "w+")  # opening GBplacesSorted.csv

    finalarray = [['% place', 'type', 'population', 'latitude', 'longitude\n']]  # this the first line from the orginal
    # format we want to keep the ordinal format so it is added as the first element

    finalarray.extend(linessorted)  # we add the sorted lines to the array

    for i in finalarray:  # writing the sorted array to the file using the same format it was written in

        filesorted.write(i[0] + "," + i[1] + "," + str(i[2]) + "," + i[3] + "," + i[4])
    
    # we close the files
    filesorted.close()
    file.close()
    print("The result is in GBplacesSorted.csv.")
    
    return 0


main()  # calling the main function
