# -*- mode: ruby -*-
# vi: set ft=ruby :

VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
  config.vm.box = "precise64"
  config.vm.box_url = "http://files.vagrantup.com/precise64.box"

  config.vm.network :private_network, ip: "192.168.66.60"
  config.vm.network "forwarded_port", guest: 80, host: 8080, auto_correct: true
  config.vm.synced_folder '.', '/vagrant', nfs: true

  config.vm.provider :virtualbox do |vb|
    vb.name = "irestful-softwares"
    vb.customize ["modifyvm", :id, "--memory", "1024"]
    vb.customize ["modifyvm", :id, "--cpus", "2"]
    vb.customize ["modifyvm", :id, "--ostype", "Ubuntu_64"]
  end
  config.vm.provision "shell", inline: <<-shell

    API_URL="http://softwares.irestful.com";
    AUTOLOAD_FILEPATH="/vagrant/vendor/autoload.php";
    DB_USERNAME="root";
    DB_PASSWORD=`tr -dc A-Za-z0-9 < /dev/urandom | head -c 8 | xargs`;
    DB_SERVER="127.0.0.1";
    DB_DRIVER="mysql";

    export LANGUAGE=en_US.UTF-8;
    export LANG=en_US.UTF-8;
    export LC_ALL=en_US.UTF-8;
    locale-gen en_US.UTF-8;
    dpkg-reconfigure locales;

    echo "export API_URL=$API_URL" >> /home/vagrant/.profile;
    echo "export AUTOLOAD_FILEPATH=$AUTOLOAD_FILEPATH" >> /home/vagrant/.profile;
    echo "export DB_USERNAME=$DB_USERNAME" >> /home/vagrant/.profile;
    echo "export DB_PASSWORD=$DB_PASSWORD" >> /home/vagrant/.profile;
    echo "export DB_SERVER=$DB_SERVER" >> /home/vagrant/.profile;
    echo "export DB_DRIVER=$DB_DRIVER" >> /home/vagrant/.profile;
    source /home/vagrant/.profile;

    sudo -s;

    echo 'alias composer="hhvm -v ResourceLimit.SocketDefaultTimeout=30 -v Http.SlowQueryThreshold=30000 -v Eval.Jit=false -v Repo.Central.Path=/var/tmp /vagrant/composer.phar"' >> /home/vagrant/.bash_aliases;
    echo 'alias phpunit="hhvm -v Eval.Jit=false -v Eval.EnableHipHopSyntax=true -v Repo.Central.Path=/var/tmp /vagrant/vendor/bin/phpunit"' >> /home/vagrant/.bash_aliases;
    . /home/vagrant/.bash_aliases;

    sudo apt-get update;
    sudo apt-get upgrade;
    sudo sudo apt-get install python-software-properties -y --force-yes;
    sudo apt-get install software-properties-common -y --force-yes;

    sudo add-apt-repository ppa:mapnik/boost;
    sudo apt-key adv --recv-keys --keyserver hkp://keyserver.ubuntu.com:80 0x5a16e7281be7a449;
    sudo echo deb http://dl.hhvm.com/ubuntu precise main | sudo tee /etc/apt/sources.list.d/hhvm.list;
    sudo apt-get update;
    sudo apt-get upgrade;

    sudo apt-get install hhvm -y --force-yes;
    sudo apt-get install curl -y --force-yes;
    sudo apt-get install git -y --force-yes;
    sudo apt-get install nginx -y --force-yes;

    sudo debconf-set-selections <<< "mysql-server-5.5 mysql-server/root_password password $DB_PASSWORD";
    sudo debconf-set-selections <<< "mysql-server-5.5 mysql-server/root_password_again password $DB_PASSWORD";
    sudo apt-get update;
    sudo apt-get install mysql-server-5.5 php5-mysql -y --force-yes;

    sudo touch ~/.hhconfig;

    #remove dependencies:
    sudo rm -R -f /vagrant/vendor;

    #download composer and install the dependencies:
    cd /vagrant; sudo curl -sS https://getcomposer.org/installer | sudo hhvm --php -dHttp.SlowQueryThreshold=30000;
    cd /vagrant; sudo hhvm -v ResourceLimit.SocketDefaultTimeout=30 -v Http.SlowQueryThreshold=30000 -v Eval.Jit=false -v Repo.Central.Path=/var/tmp /vagrant/composer.phar install --prefer-source;

    #install the custom nginx files:
    sudo cp /vagrant/configs/code.nginx.conf /etc/nginx/sites-available/code.nginx.conf;
    sudo ln -s /etc/nginx/sites-available/code.nginx.conf /etc/nginx/sites-enabled/code.nginx.conf;

    sudo cp /vagrant/configs/hack.languages.nginx.conf /etc/nginx/sites-available/hack.languages.nginx.conf;
    sudo ln -s /etc/nginx/sites-available/hack.languages.nginx.conf /etc/nginx/sites-enabled/hack.languages.nginx.conf;

    sudo cp /vagrant/configs/softwares.nginx.conf /etc/nginx/sites-available/softwares.nginx.conf;
    sudo ln -s /etc/nginx/sites-available/softwares.nginx.conf /etc/nginx/sites-enabled/softwares.nginx.conf;

    sudo cp /vagrant/configs/wildcard.softwares.nginx.conf /etc/nginx/sites-available/wildcard.softwares.nginx.conf;
    sudo ln -s /etc/nginx/sites-available/wildcard.softwares.nginx.conf /etc/nginx/sites-enabled/wildcard.softwares.nginx.conf;

    //Make sure to modify the environment variables:
    sudo sed -i "s/{username}/$DB_USERNAME/g" /etc/nginx/sites-enabled/wildcard.softwares.nginx.conf;
    sudo sed -i "s/{password}/$DB_PASSWORD/g" /etc/nginx/sites-enabled/wildcard.softwares.nginx.conf;
    sudo sed -i "s/{server}/$DB_SERVER/g" /etc/nginx/sites-enabled/wildcard.softwares.nginx.conf;
    sudo sed -i "s/{driver}/$DB_DRIVER/g" /etc/nginx/sites-enabled/wildcard.softwares.nginx.conf;

    //restart the server:
    sudo service nginx restart;

    #install hhvm in fast-cgi:
    sudo /usr/share/hhvm/install_fastcgi.sh;

    #delete the bin directory:
    rm -R -f /vagrant/bin;

    #create the web directories for our apps:
    mkdir /vagrant/bin;
    mkdir /vagrant/bin/softwares;
    mkdir /vagrant/bin/softwares/database;
    mkdir /vagrant/bin/softwares/bin;
    mkdir /vagrant/bin/softwares/wildcard;
    mkdir /vagrant/bin/softwares/wildcard/bin;
    mkdir /vagrant/bin/languages;
    mkdir /vagrant/bin/languages/hack;
    mkdir /vagrant/bin/languages/hack/bin;
    mkdir /vagrant/bin/languages/hack/cache;

    #link the web files:
    ln -s /vagrant/vendor/irestful/core/src/softwares.hh /vagrant/bin/softwares/bin/index.hh;
    ln -s /vagrant/vendor/irestful/core/src/wildcard.softwares.hh /vagrant/bin/softwares/wildcard/bin/index.hh;
    ln -s /vagrant/vendor/irestful/core/src/languages/hack.hh /vagrant/bin/languages/hack/bin/index.hh;

    #add the event domain in the host file:
    sudo echo "127.0.0.1 code.irestful.com" >> /etc/hosts
    sudo echo "127.0.0.1 hack.languages.irestful.com" >> /etc/hosts
    sudo echo "127.0.0.1 softwares.irestful.com" >> /etc/hosts
    sudo echo "127.0.0.1 datastore-apis.softwares.irestful.com" >> /etc/hosts

    #execute the tests:
    cd /vagrant; sudo hhvm -v ResourceLimit.SocketDefaultTimeout=30 -v Http.SlowQueryThreshold=30000 -v Eval.Jit=false -v Repo.Central.Path=/var/tmp /vagrant/composer.phar dump-autoload --optimize;
    cd /vagrant; sudo hhvm -v Eval.Jit=false -v Eval.EnableHipHopSyntax=true -v Repo.Central.Path=/var/tmp /vagrant/vendor/bin/phpunit -c .;

  shell
end
