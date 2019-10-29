#Write a Python program that reads in the data from the file GBplaces.csv (see last week's practice) and presents it visually using
#the matplotlib module in whatever amazing way you can dream up! I encourage you to be as creative as possible and marks will be
#awarded for this. If you just plot an x-y graph of e.g. population against latitude that will score you around 50%.
#Matplotlib is pretty powerful, as you can see from the gallery page at http://matplotlib.org/gallery.html - I want you to play with it
#and make a beautiful visualisation. You can use whatever modules or elements of the language you can find out about.
#The only rule is that you have to submit a program that generates the plot, not the plot itself. I want to be able to run your program
#and have it display the plot on my screen. That means that to be safe you will have to test it on the Campus PCs using e.g. Spyder to
#make sure it works! Do not use any non-standard modules not installed on the Campus PCs. matplotlib is very powerful and should be
#able to do what you want. Any additional files required for the program to run must be included in the submission, otherwise you may
#get 0%. (Assigment week 5)

#Mouse wheel to zoom in a point or out. Press e to switch between plot modes. Hover over a point to get data about the point.

import matplotlib.pyplot as plt
from mpl_toolkits.basemap import Basemap
import numpy as np
from math import cos, asin, sqrt

sc = None  # null reference phyton variables are references in the end anyway
infobox = None  # null reference
fig = None  # null reference
axis = None  # null reference
settlementssorted = None  # null reference will contain settlements sorted according popultion
townssorted = None # null reference will contain towns sorted according popultion
citiessorted = None # null reference will contain cities sorted according popultion
m = None # null refrence this will contain the map
plot_mode = 0 # there is 3 plot_mode all(0),cities(1),towns(2)


class Settlement:
    # will hold the data of a city/town
    def __init__(self, array): # constructor of the Settlement class
        self.name = array[0]
        self.type = array[1]
        self.lat = array[3]
        self.long = array[4]
        self.population = array[2]

def distance(lat1, long1, lat2, long2):
    # calculating the distance using Haversine forumla
    # https://en.wikipedia.org/wiki/Haversine_formula
    p = np.pi/180 # Pi/180
    a = 0.5 - cos((lat2-lat1)*p)/2 + cos(lat1*p)*cos(lat2*p) * (1-cos((long2-long1)*p)) / 2
    # Haversine formula
    return 12742 * asin(sqrt(a)) # 2 * R * asin
    # R = 6371 Radius of the earth in km

def closest(data, target):
    # fucntion used to calculate the closest element to the target
    temp_data = [x for x in data if x != target]
    # generating an array that doesn't contain the target itself
    return min(temp_data, key=lambda p: distance(float(target.lat),
                                            float(target.long),
                                            float(p.lat),
                                            float(p.long)))
    # returning the closest elmenent to the target in this cases it is expected
    # to be a Settlement object

def update_information_box(ind):
    # this will update the box object to the point that we are hovering over
    # the argument is the position of the point in the plotted points
    pos = sc.get_offsets()[ind['ind'][0]]
    # getting the position of the point
    infobox.xy = pos
    # setting the postion of the infobox to the point
    text = 'Name: {0}\nType:{1}\nPopulation: {2}\nRanking in population: {3}\nLattitude, longitude: {4}\nClosest Town/City:{5}'
    global plot_mode
    default_array = None
    if plot_mode == 0:
        default_array = settlementssorted
    elif plot_mode == 1:
        default_array = citiessorted
    else:
        default_array = townssorted
    text = text.format(str(default_array[ind['ind'][0]].name),
                       str(default_array[ind['ind'][0]].type),
                       str(default_array[ind['ind'][0]].population),
                       str(ind['ind'][0]+1),
                       str(default_array[ind['ind'][0]].lat) + ' ' +
                       str(default_array[ind['ind'][0]].long),
                       str(closest(default_array,default_array[ind['ind'][0]]).name))
    # we get out the data from the array (depends on the plot mode)
    # the memebers of the arrays are expected to be Settlements
    # we only plot the biggest population settlement that is the first element
    # of the ind always
        
    infobox.set_text(text)
    infobox.get_bbox_patch().set_facecolor('green')
    infobox.get_bbox_patch().set_alpha(0.7)
    # set the infobox test and we set colour and trasparency


def hover(event):
    # reacts to mouse movement and displays data of a point if we are over the
    # point
    if sc is None:
        print('sc is null they should be set before this function is called')
        return 0
    if infobox is None:
        print('annot is null they should be set before this function is called')
        return 0
    if fig is None:
        print('fig is null they should be set before this function is called')
        return 0
    if axis is None:
        print('axis is null they should be set before this function is called')
        return 0
    vis = infobox.get_visible()
    # check if the infobox is visible
    if event.inaxes == axis:
        cont, ind = sc.contains(event)
        # contain returns the position of the point 
        # in the array(which is the ranking in descending order)
        # actually returns an array of the points that are in the collision box
        # of the mouse
        if cont:
            update_information_box(ind)
            infobox.set_visible(True)
            fig.canvas.draw_idle()
        else:
            if vis:
                infobox.set_visible(False)
                fig.canvas.draw_idle()
        # if there is nothing colliding with the mouse we check if the infobox
        # is visible and if it is we make it invisible

def set_plot_mode(event):
    # changes the plote mode so we can display all the settlements
    # or just the cities or just the towns
    default_data = None
    
    if event.key == 'e':
        colour = '#CD3333'
        
        global plot_mode
        plot_mode += 1
        if plot_mode > 2:
            plot_mode = 0
        if plot_mode == 0:
            default_data = settlementssorted
            colour = point_colour(settlementssorted)
        elif plot_mode == 1:
            default_data = citiessorted
            colour = '#CD3333'
        else:
            default_data = townssorted
            colour = '#3D59AB'
        # pressing e increses plot_mode if it's bigger than 2 then it goes to 0
        # if plot_modes is 0 we plot every settlement if it's 1 only cities
        # if it is 2 then only towns
        if default_data is None:
            print('default_data is null, it should be set before this fucniton is called')
            return 0

        x = np.array(list(map(lambda v: float(v.long), default_data)))
        # the lambda function takes out the long value from the settlement and the
        # map function loops through settlementssorted which contains
        # the settlement objects
        y = np.array(list(map(lambda v: float(v.lat), default_data)))
        # same as the long but now we get the lat member of the settlement
        
        global m
        if m is None:
            print('m is null, it should be set before this fucniton is called')
            return 0
        
        x_projected, y_projected = m(x,y)
        global sc
        sc.remove()
        # we remove the previous points so we can add the new ones
        sc = plt.scatter(x_projected, y_projected, c=colour	, s=100,zorder=2)
        plt.draw() # redraws

scale = 1.5  # scale changes the amount of zoom


def zoom(event):
    # function used to zoom in or zoom out on a point
    # get the current x and y limits
    current_xlimit = axis.get_xlim()
    current_ylimit = axis.get_ylim()
    current_xrange = (current_xlimit[1] - current_xlimit[0])*.5
    current_yrange = (current_ylimit[1] - current_ylimit[0])*.5
    # range from the point of zoom is the half diffrence between the two limits
    x = event.xdata  # get event x location
    y = event.ydata  # get event y location
    if event.button == 'up':
        # zoom in
        scale_factor = 1/scale
    elif event.button == 'down':
        # zoom out
        scale_factor = scale
    else:
        # something that should never happen
        scale_factor = 1
        print(event.button)
    # set new limits
    axis.set_xlim([x - current_xrange*scale_factor,
                 x + current_xrange*scale_factor])
    axis.set_ylim([y - current_yrange*scale_factor,
                 y + current_yrange*scale_factor])
    plt.draw()  # redraws the plot


def connect_zoom(ax):
    global fig
    fig = ax.get_figure()  # get the figure that we want to scale
    # attach the call back
    fig.canvas.mpl_connect('scroll_event', zoom)


def openfile(string, mode):
    # checks if file can be opened
    # returns true if it can be
    # takes two string as argument

    try:
        file = open(string, mode)
        file.close()
        return True

    except:
        print('File ' + string + ' cannot be opened!')

        return False

def isnumber(arg):
    # function used to check the input whether 
    # it is convertible to float or not

    try:
        float(arg)
        return True
    
    except:
        return False


def sorting_population(file):
    # takes a reference to a file with text
    # sorts the lines in ascending order according to the second column if the 
    # column contains numbers
    # returns the sorted array if there is an error it returns an empty array

    settlements = []  # array made to contain settlements(all)
    cities = [] # array made to contain settlements(cities)
    towns = []  # array made to contain settlements(towns)
    for line in file:
        line = line.rstrip('\n') # removing \n from the line
        linearray = line.split(',')  # splitting the line
        if linearray[0] == '% place':  
            # we skip the this element it contains no data
            continue
        if linearray[1] == "City":
            cities.append(Settlement(linearray))
        else:
            towns.append(Settlement(linearray))
        settlements.append(Settlement(linearray)) 
        # we add the settlement to our array
        # plus we make to more arrays so we can track towns and cities seprate

    for x in settlements:  
        # checking each line if it contains numbers in the population

        if not isnumber(x.population):

            print('The population contains non integer elements.')

            print(x.name)

            return []
        
        if float(x.population) < 1:
            
            print('The population contains non positive integer elements.')
            
            print(x.name)
            
            return []

        if not isnumber(x.lat):

            print('The lattitude contains non integer elements.')

            print(x.name)

            return []
        
        if not isnumber(x.long):

            print('The longitude contains non integer elements.')

            print(x.name)

            return []
    global townssorted, citiessorted
    townssorted = sorted(towns, reverse=True, key=lambda x: float(x.population))
    citiessorted = sorted(cities, reverse=True, key=lambda x: float(x.population))
    return sorted(settlements, reverse=True, key=lambda x: float(x.population))
    # we use the sorting function and we give the lines as input
    # then we give a function(lambda) that converts every settlement's 
    # population member to a float we could convert to
    # integer too if we want

def point_colour(array):
    # makes an array of 2 colours red if it is a city blue if it is a town
    # green should never happen it just away to see if there is an error in 
    # the data
    cols=[]
    for x in array:
        if x.type == 'City':
            cols.append('#CD3333')
        elif x.type == 'Town':
            cols.append('#3D59AB')
        else:
            cols.append('green')
    return cols


def main():
    # main function
    # calls a function to check if the file can be accessed or not
    # plots the data on a map
    # connects events to the interactive functions
    

    if not openfile('GBplaces.csv', 'r'):  
        # checking if GBplaces.csv can be opened if not return 0

        return 0

    file = open('GBplaces.csv', 'r')  # opening GBplaces.csv
    global settlementssorted
    settlementssorted = sorting_population(file)  
    # sorting the lines inside the file and saving them in array

    if not settlementssorted:  
        # if the array is empty we say that and return 0

        print('The population could not be sorted.')

        return 0

    file.close()  #closing the file so we don't need it anymore
    
    global fig, axis
    fig, axis = plt.subplots(figsize=(10,20))
    
    global m
    m = Basemap(resolution='l',  # c, l, i, h, f or None
                projection='merc',
                lat_0=54.5, lon_0=-4.36,
                llcrnrlon=-5.3292, llcrnrlat= 50.2863, urcrnrlon=1.922, 
                urcrnrlat=57.5339)
    # westlimit=-5.3292; southlimit=50.2863;
    # eastlimit=1.922; northlimit=57.5339
    # function to get the map of Great Britain
    m.drawmapboundary(fill_color='#46bcec')
    m.fillcontinents(color='#f2f2f2', lake_color='#46bcec')
    m.drawcoastlines()
    
    x = np.array(list(map(lambda v: float(v.long), settlementssorted)))
    # the lambda function takes out the long value from the settlement and the
    # map function loops through settlementssorted which contains
    # the settlement objects
    y = np.array(list(map(lambda v: float(v.lat), settlementssorted)))
    # same as the long but now we get the lat member of the settlement
            
    x_projected, y_projected = m(x,y)
    # projecting the long and latituted to the map so the points will be
    # at the right position
    
    global sc
    sc = plt.scatter(x_projected, y_projected,
                     c=point_colour(settlementssorted),
                     s=100, zorder=2)
    # setting that null reference to an actual object
    # plotting the points and changing the zorder so the points are above the
    # map

    global infobox
    infobox = axis.annotate('', xy=(0, 0), xytext=(20, 20),
                          textcoords='offset points',
                          bbox=dict(boxstyle='round', fc='w'),
                          arrowprops=dict(arrowstyle='->'))
    # makes a box actually is just annotation for the points
    infobox.set_visible(False)

    fig.canvas.mpl_connect('motion_notify_event', hover)
    fig.canvas.mpl_connect('key_press_event', set_plot_mode)
    connect_zoom(axis)
    # connecting functions to different events

    plt.show()


main()  # calling the main function
