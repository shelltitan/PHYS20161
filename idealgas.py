#Write a Python function that returns the classical kinetic energy (Assigment week 2)
main_loop = 1


def isnumber(arg):
    # function used to check the input whether it is convertible to float or not

    try:

        float(arg)

        return True

    except:

        return False


def quitprogram(message):

    # checking if the user wants to stop the program
    # the message can be adjusted to the appropriate question at that point

    response = input(message)

    if response.lower() != "y":

        return 1

    else:

        return 0


def varaiablechooser():

    # checking if the given type is a valid choice
    # if too much wrong answer is given asking if the user wants to stop

    breakcounter = 0  # failed attempt counter

    response = input("Are you going to enter a[t]emperature, a[p]ressure or a[v]olume?")

    while (response.lower() != "p") and (response.lower() != "v") and (response.lower() != "t"):  # loop is true until a
        # valid input is given

        response = input("Please just enter a[t]emperature, a[p]ressure or a[v]olume.\n")
        breakcounter += 1  # increasing the counter

        if breakcounter >= 3:  # after 3 failed attempt we ask if user wants to continue
            breakcounter = quitprogram("Do you wish to continue calculation?(y else the program stops)")

            if breakcounter:
                return 0

    return response


def valueinput(typename):

    # checks if the value put in is a valid number
    # checks if it's negative (we don't want negative numbers)
    # decides which type of value we are asking for

    number = 0
    variablename = ""

    if typename.lower() == "p":
        number = input("OK please tell me the pressure in Pa")
        variablename = "Pressure"

    elif typename.lower() == "v":
        number = input("OK please tell me the volume in m^3")
        variablename = "Volume"

    else:
        number = input("OK please tell me the temperature in K")
        variablename = "Temperature"

    if not isnumber(number):
        print(variablename + " should be a positive number")
        if quitprogram("Do you want to do restart the calculation (y/n)?(anything else than y gets interpreted as n)"):
            print("Thanks for using the Ideal Gas Law calculator.Bye.")
            return -2

        else:

            return -1

    if not float(number) > 0:
        print(variablename + " should be a positive non-zero number")
        if quitprogram("Do you want to do restart the calculation (y/n)?(anything else than y gets interpreted as n)"):
            print("Thanks for using the Ideal Gas Law calculator.Bye.")
            return -2

        else:

            return -1

    else:
        return float(number)


def statecalculation(array):
    # calculates the missing quantity and prints the result
    # the equation used is the ideal gas law P * V = n * R * T, it is aways rearranged to the missing quantity
    # array contains the variable and the value given for that variable
    # [type1, value for type1, type2, value for type2]
    # prints result after calculating it

    R = 8.314  # Regnault constant
    n = 1  # we are calculating the result for 1 mole

    if array[0] == "p":

        if array[2] == "v":  # T = P * V / (n * R)
            result = array[1] * array[3] / (R * n)  # in this case array[1] contains the value for pressure
            # and array[3] contains the value for the volume
            print("The temperature of one mole of gas at %8.2e Pa and having volume %8.2e m^3 is %6.2f K." % (array[1],
                                                                                                              array[3],
                                                                                                              result))

            return 0

        else:  # V = n * R * T / P
            result = (R * n * array[3]) / array[1]  # in this case array[1] contains the value for pressure
            # and array[3] contains the value for the temperature
            print("The volume of one mole of gas at %6.2f K and having pressure %8.2e Pa is %8.2e m^3." % (array[3],
                                                                                                           array[1],
                                                                                                           result))

            return 0

    elif array[2] == "p":

        if array[0] == "v":  # T = P * V / (n * R)
            result = array[1] * array[3] / (R * n)  # in this case array[1] contains the value for volume
            # and array[3] contains the value for the pressure
            print("The temperature of one mole of gas at %8.2e Pa and having volume %8.2e m^3 is %6.2f K." % (array[3],
                                                                                                              array[1],
                                                                                                              result))

            return 0

        else:  # V = n * R * T / P
            result = (R * n * array[1]) / array[3]  # in this case array[1] contains the value for temperature
            # and array[3] contains the value for the pressure
            print("The volume of one mole of gas at %6.2f K and having pressure %8.2e Pa is %8.2e m^3." % (array[1],
                                                                                                           array[3],
                                                                                                           result))

            return 0

    else:  # V and T
        if array[0] == "v":  # P = n * R * T / V
            result = (R * n * array[3]) / array[1]  # in this case array[1] contains the value for volume
            # and array[3] contains the value for the temperature
            print("The pressure of one mole of gas at %6.2f K and having volume %8.2e m^3 is %8.2e Pa." % (array[3],
                                                                                                           array[1],
                                                                                                           result))

            return 0

        else:  # P = n * R * T / V
            result = (R * n * array[1]) / array[3]  # in this case array[1] contains the value for temperature
            # and array[3] contains the value for the volume
            print("The pressure of one mole of gas at %6.2f K and having volume %8.2e m^3 is %8.2e Pa." % (array[1],
                                                                                                           array[3],
                                                                                                           result))

            return 0


def main():
    # main function contains the main loop / program loop it is true as long as the program is running
    # collecting the information before calculation

    idealgasarray = []  # [type1, value for type1, type2, value for type2]
    # we put the given variable type then the given value
    # we do this until the length is 4 then we use this to calculate the missing quantity

    while main_loop:  # main loop

        while len(idealgasarray) < 4:  # filling up idealgasarray with the given input

            response = varaiablechooser()  # calling varaiablechooser function

            if not response:  # this is only true if the response is 0 (false) in other cases everything is true and
                # not true is false
                # not false is true

                return 0

            idealgasarray.append(response)  # we add the type to the array

            if len(idealgasarray) > 2:  # checking if the user gave the two variable twice

                if idealgasarray[0] == idealgasarray[2]:

                    print("The two variable cannot be the same")

                    if quitprogram("Do you want to do restart the calculation (y/n)? "
                                   "(anything else than y gets interpreted as n)"):

                        print("Thanks for using the Ideal Gas Law calculator.Bye.")

                        return 0

                    else:

                        idealgasarray = []  # setting the array to empty so we can restart the calculation

                        continue

            number = valueinput(response)  # calling valueinput function

            if number == -2:  # -2 is the return value if the user choose to stop the program

                return 0

            if number == -1:  # -1 is the return value if the user choose to restart the program

                idealgasarray = []

                continue

            idealgasarray.append(number)  # we add the value to the array (float)

        print("Thanks!")

        statecalculation(idealgasarray)  # calling statecalculation function

        if quitprogram("Do you want to do another calculation (y/n)? (anything else than y gets interpreted as n)"):

            print("Thanks for using the Ideal Gas Law calculator.Bye.")

            return 0

        else:

            idealgasarray = []  # setting the array to empty so we can do the new calculation

# calling main function here


main()
