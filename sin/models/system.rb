class System
  include DataMapper::Resource

  has n, :dlls

  has n, :softwares, :through => Resource

  property :id,         Serial
  property :os_name,    String
  property :os_version,  String
  property :browser_name, String
  property :browser_version, String
  property :created_at, DateTime, :default => lambda { DateTime.now }

  def proper
    self.os_name + ", " + self.browser_name #self.os_version + ", " + self.browser_name + " " + self.browser_version
  end

end
