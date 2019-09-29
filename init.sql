
CREATE TABLE IF NOT EXISTS `users` (
  `id` int(11) NOT NULL,
  `username` varchar(8) UNIQUE NOT NULL,
  `password` varchar(8) NOT NULL,
  `token` varchar(100),
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `modified_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP
);
 
ALTER TABLE `users` ADD PRIMARY KEY (`id`);
ALTER TABLE `users` MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;


CREATE TABLE IF NOT EXISTS `ads` (
  `id` int(11) NOT NULL,
  `userId` int(11) NOT NULL,
  `title` varchar(50) NOT NULL,
  `content` varchar(200) NOT NULL,
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `modified_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP
);
ALTER TABLE `ads` ADD PRIMARY KEY (`id`);
ALTER TABLE `ads` MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `ads` ADD FOREIGN KEY (`userId` ) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;