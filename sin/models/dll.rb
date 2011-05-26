class Dll
  include DataMapper::Resource

  has n, :softwares

  belongs_to :system

  property :id,         Serial
  property :system_id,  Integer
  property :version_id, Integer
  property :name,       String
  property :description, Text
  property :created_at, DateTime, :default => lambda { DateTime.now }
end
