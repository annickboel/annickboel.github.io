
CREATE TABLE IF NOT EXISTS `users` (
  `id` int(11) NOT NULL,
  `username` varchar(8) UNIQUE NOT NULL,
  `password` varchar(8) NOT NULL,
  `phone` varchar(10) NOT NULL,
  `token` varchar(100),
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `modified_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP
);
 
ALTER TABLE `users` ADD PRIMARY KEY (`id`);
ALTER TABLE `users` MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

CREATE TABLE IF NOT EXISTS `categories` (
  `id` int(11) NOT NULL,
  `name` varchar(50) UNIQUE NOT NULL,
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `modified_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP
);
 
ALTER TABLE `categories` ADD PRIMARY KEY (`id`);
ALTER TABLE `categories` MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

CREATE TABLE IF NOT EXISTS `adz` (
  `id` int(11) NOT NULL,
  `userId` int(11) NOT NULL,
  `categoryId` int(11) NOT NULL,
  `description` varchar(200) NOT NULL,
  `price` decimal(6,2) NOT NULL,
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `modified_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP
);
ALTER TABLE `adz` ADD PRIMARY KEY (`id`);
ALTER TABLE `adz` MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `adz` ADD FOREIGN KEY (`userId` ) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;
ALTER TABLE `adz` ADD FOREIGN KEY (`categoryId` ) REFERENCES `categories` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;