<?hh //strict
namespace Etsy\Services\Item;

use Plenty\Plugin\ConfigRepository;
use Plenty\Modules\Item\DataLayer\Models\Record;

use Etsy\Helper\ItemHelper;
use Etsy\Api\Client;
use Etsy\Logger\Logger;
use Etsy\Services\Item\ListingImageService;
use Etsy\Services\Item\ListingTranslationService;

class StartListingService
{
    /**
    * ConfigRepository $config
    */
    private ConfigRepository $config;

    /**
    * ItemHelper $itemHelper
    */
    private ItemHelper $itemHelper;

    /**
    * Client $client
    */
    private Client $client;

    /**
    * Logger $logger
    */
    private Logger $logger;

    /**
    * ListingImageService $imageService;
    */
    private ListingImageService $imageService;

    /**
     * ListingTranslationService $translationService
     */
    private ListingTranslationService $translationService;

    public function __construct(
        ItemHelper $itemHelper,
        ConfigRepository $config,
        ListingImageService $imageService,
        Client $client,
        Logger $logger,
        ListingTranslationService $translationService
    )
    {
        $this->itemHelper = $itemHelper;
        $this->config = $config;
        $this->client = $client;
        $this->logger = $logger;
        $this->imageService = $imageService;
        $this->translationService = $translationService;
    }

    public function start(Record $record):void
    {
        // $listingId = $this->createListing($record);

        $listingId = $this->createListingMockupResponse();

        if(!is_null($listingId))
        {
            // $this->addPictures($record, $listingId);

            // $this->addVariations($record, $group);

            $this->addTranslations($record, $listingId);

            $this->publish($listingId, $record->variationBase->id);
        }
        else
        {
            $this->logger->log('Could not start listing for variation id: ' . $record->variationBase->id);
        }

    }

    private function createListing(Record $record):?int
    {
        $itemData = [
            'state'                 => 'draft',
            'title'                 => 'Test', // get title
            'description'           => 'Description', // get description
            'quantity'              => $this->itemHelper->getStock($record),
            'price'                 => number_format($record->variationRetailPrice->price, 2),
            'shipping_template_id'  => $this->itemHelper->getItemProperty($record, 'shipping_template_id'),
            'who_made'              => $this->itemHelper->getItemProperty($record, 'who_made'),
            'is_supply'             => (string) $this->itemHelper->getItemProperty($record, 'is_supply'),
            'when_made'             => $this->itemHelper->getItemProperty($record, 'when_made')
        ];

        $response = $this->client->call('createListing', ['language' => 'de'], $itemData);

        if(is_null($response) || (array_key_exists('exception', $response) && $response['exception'] === true))
        {
            return null; // TODO  throw exception
        }

        return (int) reset($response['results'])['listing_id'];
    }

    private function addPictures(Record $record, int $listingId):void
    {
        $list = $this->itemHelper->getImageList($record->variationImageList['all_images']->toArray(), 'normal');

        foreach($list as $image)
        {
            $this->imageService->uploadListingImage($listingId, $image);
        }
    }

    private function addVariations(Record $record, array<int,Record> $group):void
    {

    }

    private function addTranslations(Record $record, int $listingId):void
    {
        //TODO add foreach for the itemDescriptionList
        $language = 'de';
        $this->translationService->createListingTranslation($listingId, $record, $language);
    }

    private function publish(int $listingId, int $variationId):void
    {
        $itemData = [
            'state' => 'active',
        ];

        $response = $this->client->call('updateListing', [
            'language' => 'de',
            'listing_id' => $listingId,
        ], $itemData);

        if(is_null($response) || (array_key_exists('exception', $response) && $response['exception'] === true))
        {
            // TODO  throw exception
        }

        $this->itemHelper->generateSku($listingId, $variationId);
    }




    private function createListingMockupResponse():int
    {
        return 465564444;
    }
}
