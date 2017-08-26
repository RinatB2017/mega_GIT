using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using ImageUploader.Resources;
using Microsoft.Phone.Tasks;
using System.IO;
using System.Windows.Media.Imaging;
using System.Net.Http;

namespace ImageUploader
{
    public partial class MainPage : PhoneApplicationPage
    {
        private MemoryStream photoStream;
        private MemoryStream sendPhotoStream;

        private string fileName;

        PhotoChooserTask photoChooserTask;

        // Constructor
        public MainPage()
        {
            InitializeComponent();

            photoChooserTask = new PhotoChooserTask();

            photoChooserTask.Completed +=

                new EventHandler<PhotoResult>(OnPhotoChooserTaskCompleted);

            // Sample code to localize the ApplicationBar
            //BuildLocalizedApplicationBar();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            var TitleProgressIndicator=new ProgressIndicator();
            TitleProgressIndicator.Text="Know Your Cards";

            SystemTray.SetProgressIndicator(this, TitleProgressIndicator);
            SystemTray.ProgressIndicator.IsVisible = true;
            base.OnNavigatedTo(e);

        }

        private void OnPhotoChooserTaskCompleted(object sender, PhotoResult e)
        {
            UploadButton.IsEnabled = false;
            // Make sure the PhotoChooserTask is resurning OK

            if (e.TaskResult == TaskResult.OK)
            {
                // initialize the result photo stream

                photoStream = new MemoryStream();
                sendPhotoStream = new MemoryStream();

                // Save the stream result (copying the resulting stream)

                e.ChosenPhoto.CopyTo(photoStream);

                // save the original file name

                fileName = e.OriginalFileName;

                BitmapImage setImage = new BitmapImage();
                setImage.SetSource(photoStream);

                WriteableBitmap wb = new WriteableBitmap(setImage);
                var resizeHeight=((double)setImage.PixelHeight / (double)setImage.PixelWidth * 640d);
                wb=wb.Resize(640, (int)resizeHeight, WriteableBitmapExtensions.Interpolation.Bilinear);
                wb.SaveJpeg(sendPhotoStream, wb.PixelWidth, wb.PixelHeight, 0, 100);

                setImage.SetSource(sendPhotoStream);

                
                SelectedImage.Source = setImage;

                // display the chosen picture
                UploadFile();

            }

            else
            {
                MessageBox.Show("Error picking picture");


            }

            UploadButton.IsEnabled = true;
        }

        private async void UploadFile()
        {
            try
            {

                if (sendPhotoStream != null)
                {
                    var fileUploadUrl = @"http://10.211.10.190:60662/api/file";

                    var client = new HttpClient();

                    // Reset the photoStream position

                    // If you don't reset the position, the content lenght

                    // sent will be 0

                    sendPhotoStream.Position = 0;

                    // This is the postdata

                    MultipartFormDataContent content = new MultipartFormDataContent();

                    content.Add(new StreamContent(sendPhotoStream), "file", fileName);

                    // upload the file sending the form info and ensure a result.

                    // it will throw an exception if the service doesn't return 

                    // a valid successful status code

                     var data=await client.PostAsync(fileUploadUrl, content)

                        .ContinueWith(async (postTask) => 
                        {
                            var response = await postTask;
                            
                            
                            try
                            {
                            postTask.Result.EnsureSuccessStatusCode();
                            }
                            catch
                            {

                            }
                            return response;

                        });

                     var contentString= await data.Result.Content.ReadAsStringAsync();
                     MessageBox.Show(contentString);

                }

            }

            catch
            {
                MessageBox.Show("Upload Error");
                

            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            photoChooserTask.Show();
        }



        // Sample code for building a localized ApplicationBar
        //private void BuildLocalizedApplicationBar()
        //{
        //    // Set the page's ApplicationBar to a new instance of ApplicationBar.
        //    ApplicationBar = new ApplicationBar();

        //    // Create a new button and set the text value to the localized string from AppResources.
        //    ApplicationBarIconButton appBarButton = new ApplicationBarIconButton(new Uri("/Assets/AppBar/appbar.add.rest.png", UriKind.Relative));
        //    appBarButton.Text = AppResources.AppBarButtonText;
        //    ApplicationBar.Buttons.Add(appBarButton);

        //    // Create a new menu item with the localized string from AppResources.
        //    ApplicationBarMenuItem appBarMenuItem = new ApplicationBarMenuItem(AppResources.AppBarMenuItemText);
        //    ApplicationBar.MenuItems.Add(appBarMenuItem);
        //}
    }
}