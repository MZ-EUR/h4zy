class Campaign
  include DataMapper::Resource

  belongs_to :user
  belongs_to :adtype

  has n, :ads
  has n, :urls
  has n, :keywords

  property :id,         Serial
  property :adtype_id,  Integer
  property :user_id,    Integer
  property :budget,     Integer, :default => 0
  property :paused,     Boolean
  property :is_deleted, Boolean
  property :title,      String
  property :created_at, DateTime, :default => lambda { DateTime.now }

  def status
    if self.paused then
      'Paused'
    else
      'Active'
    end
  end

end
