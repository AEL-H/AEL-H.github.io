# adapted from : http://nikolak.com/pyqt-qt-designer-getting-started/
# adapted from : https://github.com/bokeh/bokeh/blob/master/examples/glyphs/maps.py
# icon from : http://www.flaticon.com/free-icon/compass-with-cardinal-points_48647

# import various bokeh tools we will need
from bokeh.browserlib import view
from bokeh.document import Document
from bokeh.embed import file_html
from bokeh.models.glyphs import Circle
from bokeh.models import (
    GMapPlot, Range1d, ColumnDataSource,
    PanTool, WheelZoomTool,
    GMapOptions, HoverTool)
from bokeh.resources import INLINE

import math # needed for setting circle sizes using math.sqrt()
from PyQt4 import QtGui # Import the PyQt4 module we'll need
import sys #  Needed to pass argv to QApplication and for sys.exit()

import Design # This file holds our MainWindow and all design related things

## preliminary things here done first

# try to open the csv file
try:
	readFile = open('GBplaces.csv','r');
except:
	# display what is wrong on console, then exit program
	print("Problem opening GBplaces.csv, please check file is present.\nExiting program!\n");
	sys.exit(1);


# only get here if file opened successfully.

# plot ranges to be used later
x_range = Range1d();
y_range = Range1d();

# prepare arrays to be populated
Place = [];
Type = [];
DisplayedPopulation = [];
Latitude = [];
Longitude = [];
Size = [];

# reading in from file

# store the header in a variable and move onto the next line
# storing it as a variable just incase in further revisions it becomes necessary
Header = next(readFile);

# loop through each line in the file
for line in readFile:
    # split the line into its respective fields
    # using ',' as the delimiter
    Fields = line.split(',');

    # populate the arrays with the corresponding fields

    Place.append(Fields[0]);
    Type.append(Fields[1]);

    # use a format specifier to separate population into thousands using commas
    # used for nicely displaying the data later
    DisplayedPopulation.append('{:,}'.format(int(Fields[2])));

    # convert latitude and longitude from string to floating point numbers
    Latitude.append(float(Fields[3]));
    Longitude.append(float(Fields[4]));

    # create a size variable which we will use to dictate the size of a circle later
    # this contains a scaling factor [exp(-4)] and is dependent on the sqrt of population
    # this is so that the area of the circle is proportional to the population
    Size.append(math.exp(-4)*math.sqrt(int(Fields[2])));

# after the arrays have been populated, get the average latitude and longitude values
# this is so that the map will later be centred on these coordinates
meanLat = sum(Latitude) / float(len(Latitude));
meanLong = sum(Longitude) / float(len(Longitude));

# at this point, all necessary data from the file has been loaded into variables
# we can close the file

readFile.close();

## here marks the end of the preliminary things to do in the program
######################################################################################

# this is the main window of the GUI
class MainApp(QtGui.QMainWindow, Design.Ui_MainWindow):

    # class function declarations below

    # initial setup of the window
    def __init__(self):

        # Super is used here to allow us to
        # access variables, methods etc in the Design.py file
        super(MainApp, self).__init__();
        self.setupUi(self);  # This is defined in Design.py file automatically
                             # It sets up layout and widgets that are defined

        # call to handleForm() when button is clicked on window
        self.generateButton.clicked.connect(self.handleForm);


    # function that brings up a messagebox given a string message
    def printDialog(self,message):
        
        box = QtGui.QMessageBox();
        box.setText(message);
        box.setWindowTitle("GBPlaces");
        box.exec_();
        return;


    # simple function that quickly checks the filename given can be written to
    # no need to remove the file afterwards, the file will be rewritten later on
    def checkFile(self, filename):

        try:
            open(filename, 'w').close();
            # it is not necessary to remove the file, as it is being used later
            return True;
        except:
            # let the user know there is a problem
            # most likely the filename is not valid, but could also be insufficient permissions
            self.printDialog('Filename is not valid, or insufficient permission to write file here.');
            return False;

    # simple function that checks the title given is valid
    # bokeh's file_html() has problems with certain encodings on the title
    # for example when I tested a title that used the arabic alphabet
    # this function should be run assuming that valid filename is given
    # therefore checkFile should be called first if not confident on filename
    def checkTitle(self,Title,filename):

        try:
            doc = Document();
            # create empty document as filename
            with open(filename, "w") as f:
                f.write(file_html(doc, INLINE, Title+" Google Maps"));
            return True;
        except:
            # bokeh's file_html has problems with certain encodings on the title
            self.printDialog('Title is not valid.');
            return False;

    # function to handle the inputs
    def handleForm(self):

        maptype="";
        
        # change maptype to the appropriate radiobutton if checked
        if self.hybridButton.isChecked():
            maptype="hybrid";
        if self.roadmapButton.isChecked():
            maptype="roadmap";
        if self.satelliteButton.isChecked():
            maptype="satellite";
        if self.terrainButton.isChecked():
            maptype="terrain";

        # check a maptype has been selected
        if maptype=="":
            self.printDialog("Please select a map type.");
            return;

        Title = "";
        Title = self.titleTextBox.text(); # retrieve title user has given

        # check a title has been selected
        if Title == "":
            self.printDialog("Please give a title name.");
            return;

        user_file_name = "";
        user_file_name = self.fileTextBox.text(); # retrieve filename user has given

        # check user has given a file name
        if user_file_name == "":
            self.printDialog("Please give a filename.");
            return;

        filename = user_file_name+".html"; # append .html onto their filename
        fileValid = self.checkFile(filename);

        # check that filename is valid and we can write here
        if fileValid==False:
            # not valid --> return
            return;

        TitleValid = self.checkTitle(Title,filename);

        # check that title is valid
        if TitleValid==False:
            # not valid --> return
            return;

        # all input has been validated, we can now pass control flow to generatePlot()
        self.generatePlot(maptype,Title,filename);
        return;


    def generatePlot(self,maptype,Title,filename):

        # prepare the map
        # map is centred on the mean (lat,lon) as calculated earlier

        # set an initial zoom where coordinates are reasonably accurate
        # due to google maps using a mercator projection, when map is zoomed out
        # excessively, the coordinates begin to drift from their true locations
        map_options = GMapOptions(lat=meanLat, lng=meanLong, map_type=maptype, zoom=7);
        plot = GMapPlot(
            x_range=x_range, y_range=y_range,
            map_options=map_options,
            title=Title
        );
        
        # set up the data 
        source = ColumnDataSource(
            data=dict(
                lat=Latitude,
                lon=Longitude,
                size=Size,
                pop=DisplayedPopulation,
                place = Place,
                cityType = Type
            )
        );
        
        # make a circle, one for each place
        # apply some fill alpha, so that bigger circles do not obscure smaller circles from view
        circle = Circle(x="lon", y="lat", size="size", fill_color="red", fill_alpha=0.7, line_color="black");
        plot.add_glyph(source, circle);
        
        pan = PanTool();
        wheel_zoom = WheelZoomTool();
        hover = HoverTool(
                        tooltips = [
                                    ("Type","@cityType"),
                                    ("Place","@place"),
                                    ("Population","@pop"),

                                    # display latitude and longitude to 5 decimal places
                                    # to respresent full information of file
                                    ("Latitude","@lat{11.11111}"),
                                    ("Longitude","@lon{11.11111}"),
                                    ]
                        );
        
        plot.add_tools(pan, wheel_zoom, hover);
        
        # make the document
        doc = Document();
        doc.add(plot);
        
        try:
            # create the document as html file
            with open(filename, "w") as f:
                f.write(file_html(doc, INLINE, Title+" Google Maps"));
            view(filename);
            
            # let the user know program ran successfully
            print("Wrote to %s successful!" %(filename));
        except:
            # file and title have been checked previously so this should not really run
            # it is included here as a precaution to give user more of an idea of what has gone wrong
            # if it should ever run
            self.printDialog("Error writing the html file.");

        return;

##############################################################################################

# main program starts here
# just runs the GUI, the rest is handled from within the class above.

app = QtGui.QApplication(sys.argv);              # A new instance of QApplication
app.setWindowIcon(QtGui.QIcon('icon.png'));      # load the icon file, program will still run if not loaded
form = MainApp();                                # We set the form to be our MainApp (design)
form.show();                                     # Show the form
app.exec_();                                     # and execute the app
