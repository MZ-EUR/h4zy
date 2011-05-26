class Software
  include DataMapper::Resource

  belongs_to :dll

  has n, :systems, :through => Resource

  property :id,         Serial
  property :dll_id,     Integer
  property :name,       String
  property :version,    String
  property :path,       String
  property :browser,    String
  property :os_vers,    String
  property :created_at, DateTime, :default => lambda { DateTime.now }
end
